#include <stdio.h>


void merge(int arr[], int left, int mid, int right){
    int n1 = mid-left+1;
    int n2 = right-mid;
    int L[n1], R[n2];

    for(int i=0; i<n1; i++) L[i] = arr[left+i];
    for(int i=0; i<n2; i++) R[i] = arr[mid+1+i];

    int l=0, r=0, k=left;
    while(l<n1 && r<n2) {
        if(L[l] > R[r]) {arr[k] = R[r]; k++; r++;}
        else {arr[k] = L[l]; k++; l++;}
    }

    while(l<n1) {arr[k] = L[l]; k++; l++;}
    while(r<n2) {arr[k] = R[r]; k++; r++;}
}


void merge_sort(int arr[], int left, int right){
    if(left<right){
        int mid = (left+right)/2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid+1, right);
        merge(arr, left, mid, right);
    }
}




void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high){
    int pivot = arr[high];
    int i = low-1;

    for(int j=low; j<high; j++) 
        if(arr[j] < arr[high]) 
            i++, swap(&arr[i], &arr[j]) ;
    
    swap(&arr[i+1], &arr[high]) ;
    return i+1;
}

void quick_sort(int arr[], int low, int high){
    if(low < high){
        int pivot = partition(arr, low, high) ;
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
