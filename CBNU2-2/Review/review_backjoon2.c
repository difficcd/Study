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


// 기타콘서트 개선 1차 코드
#include <stdio.h>
#include <string.h>

#define MAXN 10
#define MAXM 50

int N, M;
char name[55];
char playable[55];
int guitar_bitmask[MAXN];

int max_song_count = 0;
int min_guitar_count = -1;

int count_bits(int bit) {
    int count = 0;
    while (bit) {
        count += bit & 1;
        bit >>= 1;
    }
    return count;
}

int main() {
    scanf("%d %d", &N, &M);

    for (int i = 0; i < N; i++) {
        scanf("%s %s", name, playable);
        int bit = 0;
      
        for (int j = 0; j < M; j++) { 
            if (playable[j] == 'Y') {
                bit |= (1 << j);
            }
        }
        guitar_bitmask[i] = bit;
    }

    // 기타 조합을 모든 부분집합 (1 ~ 2^n -1 )로 확인
    // 기타 자체를 나타내는 것. (g1 g2 g3 => 111 )
    for (int mask = 1; mask < (1 << N); mask++) {
        int combined = 0;
        int guitar_count = 0;

        for (int i = 0; i < N; i++) 
            if (mask & (1 << i)) 
                combined |= guitar_bitmask[i];
                guitar_count++; 
            
        int song_count = count_bits(combined);

        if (song_count > max_song_count) { 
            max_song_count = song_count; 
            min_guitar_count = guitar_count;
        } 
        else if (song_count == max_song_count) {
            if (min_guitar_count == -1 || guitar_count < min_guitar_count)
                min_guitar_count = guitar_count;
        }
    }

    if (max_song_count == 0) {
        printf("-1\n");
    } else {
        printf("%d\n", min_guitar_count);
    }

    return 0;
}



// 내가 새로 개선한 코드
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int N, M;

void getguitarN(int N, int M, int demi[10]) {
  int res[1<<N]; // 2^N 
  int max;  
  int flag=0;

  for(int i=1; i<(1<<N); i++)
    res[i] =0;
  
  for(int i=0; i<N; i++)
    if(demi[i] == 0)
      flag++;
  
  
  // #001 #010 011 #100 101 110 111
  for (int mask=1; mask<(1<<N); mask++){

    
    // 각 자리수를 조사해서 1이면 해당 행과 비트마스킹...
    // 5면 5와 비트마스킹. 이 부분은 잘 됨

    // mask 와 행을 어떻게 연결 시킬지가 중요
    // 101 은 실제로는 c&a 를 택하는 걸 원하지만
    // c 가 011 에 대응하기에 불가능..
    // i+1 행은 2^(i) 에 대응하도록 만들어야 함.
    // 1행은 1, 2행은 2, 3행은 4, 4행은 8...
    // i행 = 2^(i)

    // 111 & 010 => 2행에 대해서 demi[i
    // 해당 mask 에다가 demi 와 연산시킴...
    // mask = 4 인 경우
    // 3개의 행에 대해서..
    // 100 & 001 (i=0)
    // 100 & 010 (i=1)
    // 100 & 100 (i=2)
    // => 세 번째 경우에만, res[4] |= demi[2] 실행.
    // 세 번째 행만 선택해서 mask 된 res 에 저장되어야 함.

    
    for(int i=0; i<N; i++) // 모든 행에 대해 탐색
      if (mask & (1<<i)){ // 해당하는 행만 마스킹 연산
        res[mask] |= demi[i];
      }
    
  }

  max = res[1];
  int index = 1;
  for(int i=1; i<(1<<N); i++)
    if(max < res[i]){
      max = res[i];
      index = i;
    }

  int max_count = 0;

  int temp = index;
  
  if (index != 1) {
      for(int i=(N-1); i>0; i--){
        if(temp >= (1<<i)){
          temp -= (1<<i);
          max_count++;
        }
      }
  }

  if (temp == 1)
    max_count++;
  

  if(flag == N) printf("-1");
  else printf("\n%d", max_count);

  
}



int main() {
  scanf("%d %d", &N, &M);
  char guitar[55];
  char info[55];
  int bitinfo[10][55];
  int demi[10];

  for(int j=0; j<N; j++)
    demi[j] = 0;

  for (int i = 0; i < N; i++) {
    scanf("%s %s", guitar, info);
    for (int j = 0; j < M; j++) {
      if (info[j] == 'Y')
        bitinfo[i][j] = 1;
      else
        bitinfo[i][j] = 0;
    }
  }

  // 문자열 => 이진수 결과를 정수로 변환하면 간편
  for (int i = 0; i < N; i++) 
    for (int j = 0; j < M; j++){
      demi[i] += bitinfo[i][j] * (1<<j);
    }

  

  getguitarN(N, M, demi);
}

// 위 코드는 스스로 생각함 거의
// 아래 코드는 개선 코드. 가장 큰 문제점은 long 을 안쓴거였음 (1<<N 이라 2^N 라서, long demi를 써야했음)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int N, M;

void getguitarN(int N, int M, int demi[10]) {
    int max_songs = -1;
    int min_guitars = 10;

    for (int mask = 1; mask < (1 << N); mask++) {
      int temp = 0;
      int count = 0;
      
      for (int i = 0; i < N; i++) {
        if (mask & (1 << i)) {
          temp |= demi[i]; 
          count++;          
        }
      }

      int song_count = 0;
      
      for (int b = 0; b < M; b++) {
        if (temp & (1 << b))
          song_count++; // 연주가능곡수 판단
      }

      if (song_count > max_songs) {
        max_songs = song_count; // 최대곡수 갱신
        min_guitars = count; // 기타 수 갱신
      } 
      else if (song_count == max_songs) { // 같은 경우에는
        if (count < min_guitars) // 기타 수가 적은경우로 갱신
          min_guitars = count; 
      }
    }

    if (max_songs == 0)
      printf("-1\n");
    else
      printf("%d\n", min_guitars);
  }



int main() {
  scanf("%d %d", &N, &M);
  char guitar[55];
  char info[55];
  int bitinfo[10][55];
  int demi[10];

  for(int j=0; j<N; j++)
    demi[j] = 0;

  for (int i = 0; i < N; i++) {
    scanf("%s %s", guitar, info);
    for (int j = 0; j < M; j++) {
      if (info[j] == 'Y')
        bitinfo[i][j] = 1;
      else
        bitinfo[i][j] = 0;
    }
  }

  // 문자열 => 이진수 결과를 정수로 변환하면 간편
  for (int i = 0; i < N; i++) 
    for (int j = 0; j < M; j++){
      demi[i] += bitinfo[i][j] * (1<<j);
    }

  

  getguitarN(N, M, demi);
}

