#include <stdio.h>
#include <stdlib.h>


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
    
  int cp2[15] = {42, 3, 17, 8, 23, 56, 1, 39, 12, 27, 9, 5, 48, 15, 31};

  radix_sort(cp2, 15);

  for(int i=0; i<15; i++)
    printf("%d ", cp2[i]);
  printf("\n\n");

}
