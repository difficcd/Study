#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#include <leptonica/allheaders.h>
#include "rwlock.h"

#ifndef RWLOCK
#define RWLOCK 2500
#endif


typedef struct {
	char * filepath ;
	PIX * pix ;
	int width ;
	int height ;
	int npixels ;

	PIX * rgb ;
	NUMA * histogram ;
	int nbins ;
	float * h ;
} 
img_prop ;

	
// ==== circular_queue ==== //
typedef struct {
	task_t ** elem ;
	int capacity ;
	int num ; 
	int front ; 
	int rear ; 
	int end;

	pthread_mutex_t lock;
	pthread_cond_t wait_deq;
	pthread_cond_t wait_enq;
} circular_queue ;
circular_queue * buf = 0x0 ;

int thread_cnt;  

float ** top_5;

rwlock_t * rwlock;

/*
	1  t1 t2 ... t5

	1  t1 t2 ... t5
	2  t1 t2 ... t5
*/


// ==== queue functions ==== //
void 
circular_queue_init(circular_queue * buf, int capacity) {
	buf->capacity = capacity ;
	buf->elem = (task_t **) calloc(sizeof(task_t *), capacity) ;
	buf->num = 0 ;
	buf->front = 0 ;
	buf->rear = 0 ;

	buf->end = 0; // 1 == end flag, thread clean

	pthread_mutex_init(&buf->lock, NULL);
	pthread_cond_init(&buf->wait_deq, NULL);
	pthread_cond_init(&buf->wait_enq, NULL);
}

void 
circular_queue_enqueue(circular_queue * buf, task_t * task) {
	pthread_mutex_lock(&buf->lock);

	while(!(buf->num < buf->capacity)){
		pthread_cond_wait(&buf->wait_enq, &buf->lock);
	}
	buf->elem[buf->rear] = task ;
	buf->rear = (buf->rear + 1) % buf->capacity ; 
	buf->num += 1 ;

	pthread_cond_signal(&buf->wait_deq);
	pthread_mutex_unlock(&buf->lock);
}

task_t * 
circular_queue_dequeue(circular_queue * buf) {
	task_t * task = 0x0 ;

	pthread_mutex_lock(&buf->lock);

	while(!(buf->num > 0) && !buf->end){
		pthread_cond_wait(&buf->wait_deq, &buf->lock);
	}

	if(!(buf->num > 0) && buf->end){
		pthread_mutex_unlock(&buf->lock);
		return NULL;
	}

	task = buf->elem[buf->front] ;
	buf->front = (buf->front + 1) % buf->capacity ;
	buf->num -= 1 ;
	
	pthread_cond_signal(&buf->wait_enq);
	pthread_mutex_unlock(&buf->lock);

	return task ;
}


// ==== threading functions ==== //

typedef struct {
	img_prop * img1 ;
	img_prop * img2 ;
	int corpus_idx;
} task_t ;

void producer(img_prop * img1, img_prop * img2, int idx){

	// img1, img2 받아서 task 포장해서 큐에 넣어줌

	task_t * task = malloc(sizeof(task_t));

	task->img1 = malloc(sizeof(img_prop *));
	task->img2 = malloc(sizeof(img_prop *));
	task->img1 = img1;
	task->img2 = img2;
	task->copus_idx = idx;

	circular_queue_enqueue(&buf, &task);
}

void * worker(){
	
	// 큐에서 비교단위 빼서 while(1) loop

	while(1) {
		task_t * temp = circular_queue_dequeue(&buf);

		if(!temp) return NULL; 
		img_processing(temp);

		free(temp->img1);
		free(temp->img2);
		free(temp);
	}

}

void img_processing(task_t * task){

	// rlock 걸어서 destroy 방지 
	read_lock(&rwlock[task->corpus_idx]);

	/* 
		img1.histogram = pixGetRGBHistogram(img1.rgb, 6, 1) ;
		img1.nbins = numaGetCount(img1.histogram) ;
		img1.h = numaGetFArray(img1.histogram, L_COPY) ;
	*/

	// rlock 풀어줌
	read_unlokc(&rwlock[task->corpus_idx]);

}



// ==== img lib functions ==== //

int img_prop_init (img_prop * p, char * filepath) 
{
	p->filepath = strdup(filepath) ;
	p->pix = pixRead(p->filepath) ;
	if (p->pix == NULL) {
		fprintf(stderr, "Fail to read pixels from a fail\n") ;
		return 1 ;
	}
	p->width = pixGetWidth(p->pix) ;
	p->height = pixGetHeight(p->pix) ;
	p->npixels = p->width * p->height ;
	p->rgb = pixConvertTo32(p->pix) ;
	p->histogram = NULL ;
	p->h = NULL ;

	return 0 ;
}

int img_prop_destroy (img_prop * p)
{
	if (p->filepath) {
		free(p->filepath) ;
		p->filepath = NULL ;
	}
	if (p->pix) {
		pixDestroy(&(p->pix)) ;
		p->pix = NULL ;
	}
	if (p->rgb) {
		pixDestroy(&(p->rgb)) ;
		p->rgb = NULL ;
	}
	if (p->histogram) {
		numaDestroy(&(p->histogram)) ;
		p->histogram = NULL ;
	}
	if (p->h) {
		free(p->h) ;
		p->h = NULL ;
	}
	return 0 ;
}

float L1_distance_sim (float * h1, float * h2, int npixels, int nbins) 
{
	float common = 0.0;
	for (l_int32 i = 0; i < nbins; i++)
		common += (h1[i] < h2[i]) ? h1[i] : h2[i];
	float sim = common / ((float) npixels) ;
	return sim ;
}


int main (int argc, char ** argv)
{
	// ./imgcmp copurs-rev  file1~4  thread_cnt 

	if(argc < 4){
		printf("format: ./imgcmp <copurs_dir> <file1~4> (thread_cnt)\n");
	} 

	img_prop img1 ;
	img_prop img2 ;


	// files arr setting
	int file_cnt = argc - 3;
	char ** files = malloc(file_cnt * sizeof(char *));

	for(int i=0; i<file_cnt; i++){
		files[i] = argv[2+i];
	}

	top_5 = malloc(file_cnt * sizeof(float *));
	for(int i=0; i<file_cnt; i++){
		top_5[i] = malloc(5 * sizeof(float));
	}


	// file_cnt : (1,4)
	char file2[1024] = { 0 } ; // copus 의 i th img (->img2)

	// rwlock_t * rwlock;
	rwlock = malloc(RWLOCK * sizeof(rwlock_t));
	for(int i=0; i<RWLOCK, i++){
		rwlock_init(&rwlock[i]);
	}

	DIR * dp = opendir(argv[1]) ;
	int debug = 0;
	int idx = 0;

	for (struct dirent * i = readdir(dp) ; debug < 10; i = readdir(dp)) {
		debug ++ ; 
		if (i->d_type == DT_REG) {
			sprintf(file2, "%s/%s", argv[1], i->d_name) ;

		img_prop_init(&img2, file2) ;
		img2.histogram = pixGetRGBHistogram(img2.rgb, 6, 1) ;
		img2.nbins = numaGetCount(img2.histogram) ;
		img2.h = numaGetFArray(img2.histogram, L_COPY) ;


		for(int i=0; i<file_cnt; i++){
			char * filename1 = files[i] ; // current arg file

			producer(&img1, &img2, idx) ;

			// worker 스레드 범위 : init~sim계산

			img_prop_init(&img1, filename1) ;

			/*
				img1.histogram = pixGetRGBHistogram(img1.rgb, 6, 1) ;
				img1.nbins = numaGetCount(img1.histogram) ;
				img1.h = numaGetFArray(img1.histogram, L_COPY) ;
				
				float score = 0.0 ;
				score = L1_distance_sim(img1.h, img2.h, img1.npixels, img1.nbins) ;
				
				printf("similarity score: %.4f %s\n", score, i->d_name);
			*/

			// wlock 을 잡고 rlock img1들(args img) read 기다려줌
			write_lock();
			img_prop_destroy(&img2) ;
			write_unlock();
		}
		idx++;
	}
		closedir(dp) ;

		img_prop_destroy(&img1) ;
	}

	return EXIT_SUCCESS;

err:
	img_prop_destroy(&img1) ;
	img_prop_destroy(&img2) ;
	return EXIT_FAILURE ;

}
