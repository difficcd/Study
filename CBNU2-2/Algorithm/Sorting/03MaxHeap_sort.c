#include <stdio.h>


void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}


/// heap_srot : heapify parts
void heapify(int arr[], int n, int i){
    int c1 = i*2+1, c2 = i*2+2;
    if(c1 > n && c2 > n) return;  
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
    for(int i = n/2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for(int i = n-1; i > 0; i--){
        swap(&arr[0], &arr[i]);   
        heapify(arr, i, 0);       
    }

}



int main() { 

  int cp1[15] = {42, 3, 17, 8, 23, 56, 1, 39, 12, 27, 9, 5, 48, 15, 31};

  heap_sort(cp1, 15); 

  for(int i=0; i<15; i++)
    printf("%d ", cp1[i]); 
  printf("\n\n");

}
