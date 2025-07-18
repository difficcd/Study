// 기수정렬 공부 (수 정렬하기 3 : 10989번)

#include <stdio.h>
#include <stdlib.h>

int getMax(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > max)
            max = arr[i];
    return max;
}

void countingSort(int arr[], int n, int exp) {
    int output[n];       // 정렬 결과를 담을 배열
    int count[10] = {0}; // 0~9 카운트
    // 자릿수별 개수 세기
    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;
    // 누적 합 (안정 정렬을 위해)
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];
    // 안정 정렬: 뒤에서부터 채우기
    for (int i = n - 1; i >= 0; i--) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }
    // 원본 배열에 복사
    for (int i = 0; i < n; i++)
        arr[i] = output[i];
}

// 기수 정렬 함수
void radixSort(int arr[], int n) {
    int max = getMax(arr, n);
    // 자릿수를 1, 10, 100 ... 순서로 정렬
    for (int exp = 1; max / exp > 0; exp *= 10)
        countingSort(arr, n, exp);
}


// 메인 함수
int main() {
    int N;
    scanf("%d", &N);
    int *arr;
    arr = (int*)malloc(sizeof(int)*N);
    for(int i=0; i<N; i++)
      scanf("%d", &arr[i]);
  
    int n = sizeof(*arr) / sizeof(arr[0]);

    radixSort(arr, n);
  
    for (int i = 0; i < n; i++)
        printf("%d\n", arr[i]);

    return 0;
}
