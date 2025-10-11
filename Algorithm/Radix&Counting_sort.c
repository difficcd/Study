#include <stdio.h>
#include <stdlib.h>


void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}


/// heap_srot : heapify parts
void heapify(int arr[], int n, int i){
    
    // k번 노드의 자식 노드는
    // k*2+1, k*2+2


    // for문 없이 i노드에대해 자식과 비교 후 재귀로 내려가는 구조만 필요
        
    int c1 = i*2+1, c2 = i*2+2;
    
    if(c1 > n && c2 > n) return;  
        // 탐색 노드를 자식 노드가 존재하는 노드에 한정 (/2 오차수정)
        // 개수로 받음. 15개니까 14까지의 인덱스만 허용 (OV방지)

    /* if(c1 <= n && c2 > n) { // 자식 노드가 1개인 경우
        if(arr[i] < arr[c1]) {
            swap(&arr[i], &arr[c1]) ;
            heapify(arr, n, c1) ;
        // 자식 노드와의 교환 이후, 상위 노드와 다시 비교: heapify 재귀
        }
        
    }
    else { // 자식 노드가 2개인 경우
        if(arr[i] < arr[c1]) {
            swap(&arr[i], &arr[c1]) ;
            heapify(arr, n, c1) ;
        }
        if(arr[i] < arr[c2]) {
            swap(&arr[i], &arr[c2]) ;
            heapify(arr, n, c2) ;
        }
    } */

    // 위와 같은 구조는 불안정함
    // (자식 두개를 교환할 때 오해의 소지가 발생할 수 있음)

    int maxidx = i;

    if(c1 < n && arr[c1] > arr[maxidx]) maxidx = c1;
    if(c2 < n && arr[c2] > arr[maxidx]) maxidx = c2;

    if(maxidx != i){
        swap(&arr[i], &arr[maxidx]);
        heapify(arr, n, maxidx);
    }

}
    

/// heap_sort : main,  max_heap 기준
void heap_sort(int arr[], int n){

    /* int cnt = 0;
    while(cnt < n){ // n회 (arr[(n-1)-cnt] 가 정렬됨)
        int idx = (n-1)-cnt;  // 이번에 정렬될 index 
        
        for(int i=0; i<n-1; i++)
            if(arr[i] < arr[i+1])      // heapify 필요조건
                heapify(arr, n-cnt, 0);  // 정렬된 부분은 제외
                
        swap(&arr[cnt], &arr[idx]);
        cnt++;
    } */

    for(int i = n/2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for(int i = n-1; i > 0; i--){
        swap(&arr[0], &arr[i]);   // 최대값을 뒤로 이동
        heapify(arr, i, 0);       // i개만 heapify (정렬된 뒤쪽 제외)
    }
    
}



int getMax(int arr[], int n){
    int max = arr[0];
    for(int i=1; i<n; i++)
        if( max < arr[i]) max = arr[i];
    return max;
    
}

 void counting_sort(int arr[], int n, int exp){
    int* output = malloc(sizeof(int) * n); //임시저장공간
    int i, count[10] = {0};

    for(i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++; 
        //해당 자릿수 카운트
     
    for(i = 1; i < 10; i++)
         count[i] += count[i-1];
        //자릿수가 i 이하인 원소의 총 개수 (누적합)
     
     for(i = n-1; i >= 0; i--){
         //count의 누적합을 이용해 뒤에서 앞으로 원소배치
         output[count[(arr[i] / exp) % 10]-1] = arr[i];
         count[(arr[i] / exp) % 10]--;
     }
     
     for(i = 0; i < n; i++)
        arr[i] = output[i];
     
     free(output);
 }


/// radix_sort : main, LSD Radix sort
void radix_sort(int arr[], int n){
    
    int max = getMax(arr, n); //최대값
    for(int exp = 1; max / exp > 0; exp *= 10)
        counting_sort(arr, n, exp); //exp : 1 10 100... %로 자릿수추출
    
}


int main() { 
    
  int cp1[15] = {42, 3, 17, 8, 23, 56, 1, 39, 12, 27, 9, 5, 48, 15, 31};
  int cp2[15] = {42, 3, 17, 8, 23, 56, 1, 39, 12, 27, 9, 5, 48, 15, 31};

  heap_sort(cp1, 15); 
  radix_sort(cp2, 15);

  for(int i=0; i<15; i++)
    printf("%d ", cp1[i]); 
  printf("\n\n");

  for(int i=0; i<15; i++)
    printf("%d ", cp2[i]);
  printf("\n\n");

}
