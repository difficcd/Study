#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#include <leptonica/allheaders.h>
#include "rwlock.h"

#ifndef CHUNK 
#define CHUNK 10
#endif

#ifndef TOP 
#define TOP 5
#endif

#ifndef CAPACITY 
#define CAPACITY 100
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

	
// ==== global variable ==== //

typedef struct {
	// img_prop * img ;
	// img_prop 구조체 전체 => 파일 경로 : task_t 가볍게 연산 수행
	char copus_path[1024];
} task_t ;

typedef struct {
	float score;
	char file_name[1024];
} top_k_t;
top_k_t ** top_k ;
// top_k[0][1].score = ..
// top_k[0][1].file_name = ..

// float ** top_k; 	// top_k[i][j] i:img_arg, j:0~4 

int thread_cnt;  
int file_cnt;

img_prop * arg_img; // arg_img[file_cnt]

char ** files;

rwlock_t rwlock;



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



/* structure example
	1  t1 t2 ... t5

	1  t1 t2 ... t5
	2  t1 t2 ... t5
*/

// ==== prod cons ==== //
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



// ==== threading functions ==== //

void producer(char * file_path){

	// task 포장해서 큐에 넣어줌

	//task_t * task = malloc(sizeof(task_t));
	//task->img = malloc(sizeof(img_prop));
	//task->img = img;

	task_t * task = malloc(sizeof(task_t));
	strcpy(task->copus_path, file_path); 

	// CHUNK 개수 만큼의 copus img 를 가져오도록 변경...
	// TODO:

	circular_queue_enqueue(buf, task);


}


void img_processing(task_t * task){

	// task->copus_path 

	img_prop img2;

	img_prop_init(&img2, task->copus_path) ;
	img2.histogram = pixGetRGBHistogram(img2.rgb, 6, 1) ;
	img2.nbins = numaGetCount(img2.histogram) ;
	img2.h = numaGetFArray(img2.histogram, L_COPY) ;


	for(int i=0; i<file_cnt; i++){
		float score = 0.0 ;
		score = L1_distance_sim(arg_img[i].h, img2.h, 
								arg_img[i].npixels, arg_img[i].nbins) ;

		write_lock(&rwlock);
		for(int j=0; j<TOP; j++){
			if(top_k[i][j].score < score) {

				for(int k=TOP-1; k>j; k--){
					top_k[i][k] = top_k[i][k-1];
				} 
				// top_k[0][1] = top_k[0][0], ... top_k[0][4] = top_k[0][3]
				// 현재 j 가 0이라면, 0 탈환, 총 top-1-0 회 k감산수행 ..


				top_k[i][j].score = score; // DESC
				strcpy(top_k[i][j].file_name, task->copus_path); 
				break;
			}
		}
		write_unlock(&rwlock);
		printf("similarity score: %.4f %s\n", score, files[i]);	
	}

	img_prop_destroy(&img2);

}

void * worker(){
	while(1) {

		task_t * temp = circular_queue_dequeue(buf);
		if(temp != NULL) img_processing(temp);
		else break;

		free(temp);
	}
}


int main (int argc, char ** argv)
{
	// ./imgcmp copurs-rev  file1~4  thread_cnt 

	if(argc < 4){
		printf("format: ./imgcmp <copurs_dir> <file1~4> <thread_cnt>\n");
	} 

	// img_prop img1 ;
	img_prop img2 ;


	// init : file cnt, thread cnt, queue init 
	file_cnt = argc - 3;
	files = malloc(file_cnt * sizeof(char *));

	for(int i=0; i<file_cnt; i++){
		files[i] = argv[2+i];
	}

	thread_cnt = atoi(argv[file_cnt+2]);
	printf("thread_cnt: %d\n", thread_cnt);

	top_k = malloc(file_cnt * sizeof(top_k_t *));
	for(int i=0; i<file_cnt; i++){
		top_k[i] = malloc(TOP * sizeof(top_k_t));
	}

	for(int i=0; i<file_cnt; i++){
		for(int j=0; j<TOP; j++){
			top_k[i][j].score = 0.0;
		}
	}

	buf = malloc(sizeof(circular_queue));
	circular_queue_init(buf, CAPACITY);


	// ==== pthread init ==== //

	pthread_t * workers;
	workers = malloc(thread_cnt * sizeof(pthread_t));
	if(workers == NULL) {
		perror("malloc failed");
	}

	rwlock_init(&rwlock);
	for(int i=0; i<thread_cnt; i++)
		pthread_create(&workers[i], NULL, worker, NULL);	
	
	arg_img = malloc(sizeof(img_prop) * file_cnt); // arg_img[file_cnt]

	char file2[1024] = { 0 } ; // copus 의 i_th img (->img2)

	// ==== args img loop ==== //

	char file1[1024] = { 0 } ;
	for(int i=0; i<file_cnt; i++){
		sprintf(file1, "%s/%s", argv[1], files[i]) ;
		img_prop_init(&arg_img[i], file1) ;

		arg_img[i].histogram = pixGetRGBHistogram(arg_img[i].rgb, 6, 1) ;
		arg_img[i].nbins = numaGetCount(arg_img[i].histogram) ;
		arg_img[i].h = numaGetFArray(arg_img[i].histogram, L_COPY) ;	
	}

	// ==== copus img loop (multi-threading main) ==== //

	DIR * dp = opendir(argv[1]) ;
	for (struct dirent * i = readdir(dp) ; i != NULL; i = readdir(dp)) {

		if (i->d_type == DT_REG) {
			sprintf(file2, "%s/%s", argv[1], i->d_name) ;

			/*
				img_prop_init(&img2, file2) ;
				img2.histogram = pixGetRGBHistogram(img2.rgb, 6, 1) ;
				img2.nbins = numaGetCount(img2.histogram) ;
				img2.h = numaGetFArray(img2.histogram, L_COPY) ;
			*/

			// task->copus_path : file2 (strcpy)
			producer(file2);
		}
	}
	closedir(dp) ;
	
	// buf->end 갱신, broadcast 둘 다 critical section!
	pthread_mutex_lock(&buf->lock);
		buf->end = 1;
		pthread_cond_broadcast(&buf->wait_deq);
	pthread_mutex_unlock(&buf->lock);

	for(int i=0; i<thread_cnt; i++)
		pthread_join(workers[i], NULL);	

	
	for(int i=0; i<file_cnt; i++){
		img_prop_destroy(&arg_img[i]) ;
	}


	printf("\n======================== result ========================\n");
	printf("========================================================\n\n");

	for(int i=0; i<file_cnt; i++){
		printf("============ file %d : %s ============\n", i+1, files[i]);
		for(int j=0; j<TOP; j++){
			printf("similarity score: %.4f %s\n", 
					top_k[i][j].score, top_k[i][j].file_name);	
		}
		printf("\n");
	}

	return EXIT_SUCCESS;

err:
	// img_prop_destroy(&img1) ;
	// img_prop_destroy(&img2) ;
	return EXIT_FAILURE ;

}
