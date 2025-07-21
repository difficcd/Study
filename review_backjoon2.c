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



// 1497번 : 기타콘서트 (적어둔대로 구현)

#include <stdio.h>
#include <stdlib.h>
/* 
11100 
00010
11001 
00011 => 나머지 두 자리를 효율적으로 메움
=> 남은 0의 위치를 모두 고려하는 로직 필요
10000

1이 제일많은걸 기준으로
0인 인덱스에 1이 들어있는 거 기준
*/

int N, M;

// M 이 비트 수 
void getguitarN(int bitinfo[][M]){
  int cnt[N]; 
  int zero[N][M];
  int idx[M];

  for(int i=0; i<N; i++){ // 초기화
    cnt[i] = 0;
    for(int j=0; j<N; j++)
      zero[i][j] = 0; 
    // 1이면 0인 곳, j가 인덱스.
  }
  
  for(int i=0; i<N; i++){
    for(int j=0; j<M; j++){
      if(bitinfo[i][j] == 0){
        zero[i][j] = 1; // 0의 개수 계산
      }
      else cnt[i]++; // 1의 개수 계산
    }
  }

  int max = cnt[0], maxidx;
  for(int i=1; i<N; i++){
    if(max < cnt[i]){
      max = cnt[i]; 
      maxidx = i; 
      // 1이 가장 많은 행(N)
    }
  }

  // maxidx(N) 을 기준으로, bitinfo 를 탐색
  // => zero == 1 인 자리를 idx 에 저장한다.
  // idx 배열에 저장된 부분에 1이 가장 많은 새로운 N을 탐색한다
  // 새로운 N과 비트마스킹 and 연산
  // 다시 zero == 1 인 자리를 idx 에 저장한다.
  
  for(int i=0; i<N; i++){
    for(int j=0; j<M; j++)
      if(i != maxidx){
        
      }
  }
  
};
  
int main() {
scanf("%d %d", &N, &M);
char guitar[55];
char info[55];
int bitinfo[55][55]; 

  for (int i = 0; i < 55; i++)
    for (int j = 0; j < 55; j++)
        bitinfo[i][j] = -1; // 빈 곳은 -1로 초기화
    
  
  for (int i = 0; i < N; i++) {
      scanf("%s %s", guitar, info); 
      for (int j = 0; j < M; j++) {
          if (info[j] == 'Y')
              bitinfo[i][j] = 1;
          else
              bitinfo[i][j] = 0;
      }
  }
  
  // 출력 확인
  for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
          printf("%d ", bitinfo[i][j]);
      }
      printf("\n");
  }
  
  getguitarN(bitinfo);
  
}



// 1497번 : 기타콘서트 (적어둔대로 구현)
// 최적해를 보장하지 않는 탐욕적 선택 for (int i = 0; i < 55; i++)
    for (int j = 0; j < 55; j++)
        bitinfo[i][j] = -1; // 빈 곳은 -1로 초기화
    
  
  for (int i = 0; i < N; i++) {
      scanf("%s %s", guitar, info); 
      for (int j = 0; j < M; j++) {
          if (info[j] == 'Y')
              bitinfo[i][j] = 1;
          else
              bitinfo[i][j] = 0;
      }
  }
  
  // 출력 확인
  for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
          printf("%d ", bitinfo[i][j]);
      }
      printf("\n");
  }
  
  getguitarN(bitinfo);
  
}



// 1497번 : 기타콘서트 (적어둔대로 구현)
// 최적해를 보장하지 않는 탐욕 알고리즘을 사용했기에 오답으로 처리되었음
// 알고리즘 공부 후 다시 도전

#include <stdio.h>
#include <stdlib.h>
/*
11100
00010
11001
00011 => 나머지 두 자리를 효율적으로 메움
=> 남은 0의 위치를 모두 고려하는 로직 필요
10000

1이 제일많은걸 기준으로
0인 인덱스에 1이 들어있는 거 기준
*/

int N, M;

void getguitarN(int N, int M, int bitinfo[55][55]) {
  int cnt[N], idx[M], idxcnt[N];
  int tempresult[M];
  int flag[N]; // 탐색이후 행은 flag=1
  int tmp = 0, result = 1, clear = 0, fin = 0;
  int tempcnt, temparr[M];

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++)
      if (bitinfo[i][j] == 1)
        tmp++;
  }
  if (tmp == N * M)
    printf("%d", result);
  else if (tmp == 0)
    printf("-1");
  else {

    for (int i = 0; i < M; i++) {
      idx[i] = 0;
      tempresult[i] = 0;
    }

    for (int i = 0; i < N; i++) { // 초기화
      cnt[i] = 0;
      flag[i] = 0;
      idxcnt[i] = 0;
      // 1이면 0인 곳, j가 인덱스.
    }

    for (int i = 0; i < N; i++)
      for (int j = 0; j < M; j++)
        if (bitinfo[i][j] == 1)
          cnt[i]++; // 각 행에서의 1의 개수 계산

    int max = cnt[0], maxidx = 0;
    for (int k = 1; k < N; k++) {
      if (max < cnt[k]) {
        max = cnt[k];
        maxidx = k;
        flag[maxidx] = 1;
        // 1이 가장 많은 행(N)
      }
    }

    for (int j = 0; j < M; j++)
      temparr[j] = bitinfo[maxidx][j]; // 복사

    // maxidx행 0 인 자리 idx 에 저장
    for (int j = 0; j < M; j++)
      if (bitinfo[maxidx][j] == 0)
        idx[j] = 1; // 0인 경우에만 위치 저장

    // idx 의 위치에 1이 가장 많은 새로운 N 탐색
    // 1이 가장 많은지 => idxcnt 로 판별
    for (int i = 0; i < N; i++) {
      if (flag[i] != 1) { // 탐색하지 않은 행에 대해서만
        for (int j = 0; j < M; j++) {
          if (idx[j] != 0) {
            if (bitinfo[i][j] == 1)
              idxcnt[i]++; // i행의 1 개수
          }
        }
      }
    }

    int newidx = 0; // 다음 탐색 변수
    int temp = idxcnt[0];
    for (int i = 0; i < N; i++)
      if (idxcnt[i] >= temp) {
        newidx = i; // 빈 곳에 1이 제일 많은 N(행)
        temp = idxcnt[i];
      }
    flag[newidx] = 1;

    // 새로운 N과 비트마스킹 and 연산
    // maxidx 와 newidx 행의 비트 OR 연산 => tempres 저장
    for (int j = 0; j < M; j++)
      if (bitinfo[maxidx][j] || bitinfo[newidx][j]) {
        tempresult[j] = 1;
      }

    for (int j = 0; j < M; j++)
      if (tempresult[j] == 1)
        clear++;
    if (clear == M)
      fin = -1;

    tempcnt = 0;
    for (int j = 0; j < M; j++)
      if (temparr[j] == tempresult[j])
        tempcnt++;
    if (tempcnt == M)
      fin = -1;
    else
      result++; // 루프당 증가

    // =========== LOOP ===============
    while (fin != -1) {

            for (int j = 0; j < M; j++) temparr[j] = tempresult[j]; // 복사

      for (int i = 0; i < N; i++)
        idxcnt[i] = 0; // i행의 1 개수

      for (int j = 0; j < M; j++)
        if (tempresult[j] == 0)
          idx[j] = 1; // 0인 경우에만 위치 저장
        else
          idx[j] = 0; // 이전 정보 제거

      for (int i = 0; i < N; i++) {
        if (flag[i] != 1) { // 탐색하지 않은 행에 대해서만
          for (int j = 0; j < M; j++) {
            if (idx[j] != 0) {
              if (bitinfo[i][j] == 1)
                idxcnt[i]++;
            }
          }
        }
      }

      temp = idxcnt[0];
      for (int i = 0; i < N; i++)
        if (idxcnt[i] >= temp) {
          newidx = i; // 빈 곳에 1이 제일 많은 N(행)
          temp = idxcnt[i];
        }
      flag[newidx] = 1;

      for (int j = 0; j < M; j++)
        if (tempresult[j] || bitinfo[newidx][j]) {
          tempresult[j] = 1;
        }

      clear = 0;
      for (int j = 0; j < M; j++)
        if (tempresult[j] == 1)
          clear++;
      if (clear == M)
        fin = -1;

      tempcnt = 0;
      for (int j = 0; j < M; j++)
        if (temparr[j] == tempresult[j])
          tempcnt++;

      if (tempcnt == M) {
        fin = -1; // 변하지 않았으면 종료
      } else
        result++; // 루프당 증가
    }
    printf("%d", result);
  }
}

int main() {
  scanf("%d %d", &N, &M);
  char guitar[55];
  char info[55];
  int bitinfo[55][55];

  for (int i = 0; i < 55; i++)
    for (int j = 0; j < 55; j++)
      bitinfo[i][j] = -1; // 빈 곳은 -1로 초기화

  for (int i = 0; i < N; i++) {
    scanf("%s %s", guitar, info);
    for (int j = 0; j < M; j++) {
      if (info[j] == 'Y')
        bitinfo[i][j] = 1;
      else
        bitinfo[i][j] = 0;
    }
  }

  getguitarN(N, M, bitinfo);
}


