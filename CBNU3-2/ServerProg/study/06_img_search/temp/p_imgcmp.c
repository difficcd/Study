#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>

#include <leptonica/allheaders.h>


typedef struct {
	int * route ;
	int * visited ;
	int next ;
} task_t ;


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

	pthread_mutex_t lock;
	pthread_cond_t wait_deq;
	pthread_cond_t wait_enq;
} circular_queue ;
circular_queue * buf = 0x0 ;

int thread_cnt; // argv[3] 

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

float L1_distance_sim (float * h1, float * h2, int npixels, int nbins) {
	float common = 0.0;
	for (l_int32 i = 0; i < nbins; i++)
		common += (h1[i] < h2[i]) ? h1[i] : h2[i];
	float sim = common / ((float) npixels) ;
	return sim ;
}

// u1(4) 참조하여 구조 설계 
int main (int argc, char ** argv)
{
	img_prop img1 ;
	img_prop img2 ;

	char filename1[] = "IMG001.jpg" ;
	char file2[1024] = { 0 } ;

	img_prop_init(&img1, filename1) ;
	img1.histogram = pixGetRGBHistogram(img1.rgb, 6, 1) ;
	img1.nbins = numaGetCount(img1.histogram) ;
	img1.h = numaGetFArray(img1.histogram, L_COPY) ;

	DIR * dp = opendir(argv[1]) ;
	for (struct dirent * i = readdir(dp) ; i != NULL ; i = readdir(dp)) {
		if (i->d_type == DT_REG) {
			sprintf(file2, "%s/%s", argv[1], i->d_name) ;

			img_prop_init(&img2, file2) ;
			// assume ( img1.height == img2.height && img1.width == img2.width )

			img2.histogram = pixGetRGBHistogram(img2.rgb, 6, 1) ;
			img2.nbins = numaGetCount(img2.histogram) ;
			img2.h = numaGetFArray(img2.histogram, L_COPY) ;

			float score = 0.0 ;
			score = L1_distance_sim(img1.h, img2.h, img1.npixels, img1.nbins) ;
			printf("similarity score: %.4f %s\n", score, i->d_name);

			img_prop_destroy(&img2) ;
		}
	}
	closedir(dp) ;

	img_prop_destroy(&img1) ;

	return EXIT_SUCCESS;

err:
	img_prop_destroy(&img1) ;
	img_prop_destroy(&img2) ;

	return EXIT_FAILURE ;
}
