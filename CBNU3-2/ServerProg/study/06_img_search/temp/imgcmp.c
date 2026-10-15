#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>

#include <leptonica/allheaders.h>

typedef struct {
		char * filepath ;
		PIX * pix ;
		// w,h,dpi 해상도 정보 등

		int width ;
		int height ;
		int npixels ;

		PIX * rgb ;
		NUMA * histogram ;
		int nbins ;
		float * h ;
	} 
	img_prop ;


int img_prop_init (img_prop * p, char * filepath) 
{
	p->filepath = strdup(filepath) ;
	p->pix = pixRead(p->filepath) ;
	// pixRead : 파일 경로에서 이미지 파일(png, jpeg..) 읽어
	// 			 메모리상의 PIX 구조체 객체로 만들어 반환해 줌.

	if (p->pix == NULL) {
		fprintf(stderr, "Fail to read pixels from a fail\n") ;
		return 1 ;
	}

	p->width = pixGetWidth(p->pix) ;
	p->height = pixGetHeight(p->pix) ;
	p->npixels = p->width * p->height ;
	p->rgb = pixConvertTo32(p->pix) ;
	// 객체로부터 이미지 w, h 받아온 후 총 pixels 수 도출
	// 원본 이미지를 32bit RGBA/RGB img포맷 PIX구조체로 반환

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
	// 둘 중 작은 값을 선택하도록 함
	// u4 참조하여 이해하기

	float sim = common / ((float) npixels) ;
	return sim ;
}


int main (int argc, char ** argv)
{
	img_prop img1 ;
	img_prop img2 ;

	char filename1[] = "IMG001.jpg" ;
	char file2[1024] = { 0 } ;

	img_prop_init(&img1, filename1) ; 
	// img_prop 초기화 함수 (이름, 파일경로 주면 동적할당)

	img1.histogram = pixGetRGBHistogram(img1.rgb, 6, 1) ;
	img1.nbins = numaGetCount(img1.histogram) ;
	img1.h = numaGetFArray(img1.histogram, L_COPY) ;

	// NUMA * histogram ;  int nbins ;  float * h ;

	// NUMA == NUMber Array, histogram => float * h...
	// nbins == 구간의 개수 (히스토그램 칸 개수)
	// h = 실제 히스토그램 데이터를 float * arr 형태로 가져옴
	// img1.h[0], img1.h[1], ..., img1.h[nbins - 1]

	DIR * dp = opendir(argv[1]) ;
	for (struct dirent * i = readdir(dp) ; i != NULL ; i = readdir(dp)) {

		// struct dirent == dir 내부의 개별 파일 정보담는 구조체
		// dir entry, linux/UNIX fs 에서 폴더내부 탐색 시 사용

		if (i->d_type == DT_REG) { // 파일 종류가 dir아니라 일반 file 인지 확인
			
			sprintf(file2, "%s/%s", argv[1], i->d_name) ; 
			// 경로랑 i->파일이름 합쳐서 dir/img1.jpg 만들음

			img_prop_init(&img2, file2) ;
			// assume ( img1.height == img2.height && img1.width == img2.width ) 
			// 즉, 정형화된 데이터를 보장해 줌.

			img2.histogram = pixGetRGBHistogram(img2.rgb, 6, 1) ;
			img2.nbins = numaGetCount(img2.histogram) ;
			img2.h = numaGetFArray(img2.histogram, L_COPY) ;
			// 똑같이 히스토그램 계산해 내기 (copus의 비교 대상 하나 하나)

			float score = 0.0 ;
			score = L1_distance_sim(img1.h, img2.h, img1.npixels, img1.nbins) ;
			// 유사도 계산

			printf("similarity score: %.4f %s\n", score, i->d_name);

			img_prop_destroy(&img2) ; 
			// 자원 정리

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
