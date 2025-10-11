#include <stdio.h>


/// merge : left~mid arr와 mid+1~right arr 섞는 과정
void merge(int arr[], int left, int mid, int right){
    int n1 = mid-left + 1;
    int n2 = right - mid;
    int L[n1], R[n2]; 

    for(int i=0; i<n1; i++) L[i] = arr[left+i];
    for(int i=0; i<n2; i++) R[i] = arr[mid+1+i];
    // L[n1], R[n2] setting

    // 2 4 9   3 7 9 
    // 2 vs 3 >> 2 을 정렬된 배열 arr 에 넣음
    // 4 9   3 7 9
    // 4 vs 3 >> 3 을 정렬된 배열 arr 에 넣음....

    // L[0] vs R[0] 해서 작은걸 arr[left] 넣기
    // L[0] 이 작으면, arr[left] = L[0] 
    // 이후, L[1] 부터 비교해야 함
    // arr[i] i:left~right 까지 비교 (합)


    /* int l=0, r=0;
    for(int i=left; i<right+1; i++){
        if (L[l] > R[r])
            arr[i] = R[r], r++;
        else arr[i] = L[l], l++;
    }
    
    => out of bound 문제 발생
    
    */


    int l=0, r=0;
    for(int i=left; i<right+1; i++){
        if(l < n1 && r < n2){
            if (L[l] > R[r])
                arr[i] = R[r], r++;
            else arr[i] = L[l], l++;
        }
        else if (l >= n1 && r < n2)
            arr[i] = R[r], r++;
        else arr[i] = L[l], l++;
    }

    /*
    #### 표준 병합 코드 ####
    
    while (l < n1 && r < n2) {
    if (L[l] <= R[r]) arr[k++] = L[l++];
    else arr[k++] = R[r++];
    }
    
    // 남은 요소가 있을 경우 따로 복사
    while (l < n1) arr[k++] = L[l++];
    while (r < n2) arr[k++] = R[r++];
    */    

    
}

/// merge_sort : mergesort main
void merge_sort(int arr[], int left, int right){
        if(left<right){
            int mid = (left+right)/2;
            merge_sort(arr, left, mid); 
            // 왼쪽 partition 재귀적으로 분할됨 
            merge_sort(arr, mid+1, right);
            // 오른쪽 partition 재귀적으로 분할됨
            
            merge(arr, left, mid, right);
        }    

}




void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}


/// quick_sort : pivot 결정 및 arr 재배열 함수
int partition(int arr[], int low, int high){
    int pivot = arr[high]; // 피봇은 맨 끝 값으로 정하기
    int i = low-1; // -1을 하는 이유 : index 라서, 1개가 생기면 [0]이어야 함

    for(int j=low; j<high; j++) // arr[high]가 피봇이므로 j<high
        if(arr[j] <= pivot) 
            i++, swap(&arr[i], &arr[j]);
    // i : 0 1(10) 2 3 4 5 6. 

    swap(&arr[i+1], &arr[high]);
    // 맨 끝에 있던 pivot 을 제자리(i+1인덱스에 보내줌)
    
    return i+1; // pivot 의 최종 위치 반환
}


/// quick_sort : main
void quick_sort(int arr[], int low, int high){
    
        if(low<high){
            int pivot = partition(arr, low, high);
            quick_sort(arr, low, pivot-1);
            quick_sort(arr, pivot+1, high);
        }

}



int main() { 
    
  int cp1[15] = {42, 3, 17, 8, 23, 56, 1, 39, 12, 27, 9, 5, 48, 15, 31};
  int cp2[15] = {42, 3, 17, 8, 23, 56, 1, 39, 12, 27, 9, 5, 48, 15, 31};

  merge_sort(cp1, 0, 14); 
  quick_sort(cp2, 0, 14);

  for(int i=0; i<15; i++)
    printf("%d ", cp1[i]); 
  printf("\n\n");

  for(int i=0; i<15; i++)
    printf("%d ", cp2[i]);
  printf("\n\n");

}
