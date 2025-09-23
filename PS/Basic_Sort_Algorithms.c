#include <iostream>
using namespace std;

void swap(int *A, int *B) {
  int temp = *A;
  *A = *B;
  *B = temp;
}

void bubble(int* arr, int n){
  for(int i=0; i<n-1; i++){
    for(int j=0; j<n-1-i; j++){
      if(arr[j] > arr[j+1])
        swap(&arr[j], &arr[j+1]);
    }
  }
}

void insert(int* arr, int n){
  for(int i = 1; i < n; i++){
      int newitem = arr[i];
      int j = i - 1; // 역순으로 밀어주어야 함
      while(j >= 0 && arr[j] > newitem){
        // 어디까지 미냐 ? =>> 값이 더 큰 범위 내에서 민다
        //  ex 0 1 3 8 11 "5" 9   : 5보다 큰 8, 11에서.
        //     *11이 먼저 5 자리로, 그 다음 8이 11 자리로 감
          arr[j+1] = arr[j];
          j--;
      }
      arr[j+1] = newitem; // 들어갈 자리에 item 삽입
  }
}

void select(int* arr, int n){ // 큰 수를 앞으로...
  for(int i=0; i<n-1; i++){ // 마지막은 필요없음
    int maxidx = 0;
     for(int j=0; j<n-i; j++){
       if(arr[maxidx] < arr[j])
         maxidx = j;
     }
     swap(&arr[maxidx], &arr[n-1-i]);
  }
}


int main() { 
  int cp1[5] = {1,0,3,5,9};
  int cp2[5] = {1,0,3,5,9};
  int cp3[5] = {1,0,3,5,9};
  
  select(cp1, 5); 
  bubble(cp2, 5);
  insert(cp3, 5);
  
  for(int i=0; i<5; i++)
    printf("%d ", cp1[i]); 
  printf("\n");

  for(int i=0; i<5; i++)
    printf("%d ", cp2[i]);
  printf("\n");

  for(int i=0; i<5; i++)
    printf("%d ", cp3[i]);
  printf("\n");
  
}
