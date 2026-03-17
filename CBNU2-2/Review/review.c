#include <stdio.h>

// 10818 최소 최대 간단히 복습
int maxnum(int *, int);
int minnum(int *, int);

int main() {
  int size;
  scanf("%d", &size);

  int num[size];

  for (int i = 0; i < size; i++)
    scanf("%d", &num[i]);

  int max = maxnum(num, size);
  int min = minnum(num, size);

  printf("%d %d", min, max);

  return 0;
}

int maxnum(int *num, int size) {
  int max = num[0];
  for (int i = 1; i < size; i++)
    if (max < num[i])
      max = num[i];
  return max;
}

int minnum(int *num, int size) {
  int min = num[0];
  for (int i = 1; i < size; i++)
    if (min > num[i])
      min = num[i];
  return min;
}

// 2562 9개의 수 중 최대값과 그 인덱스
// flag = 0 으로 설정해서 오답이 있었음 
// => 로직을 섬세하게 짜서 미연에 오류 방지

#include <stdio.h>
// #include <stdbool.h>
// 부울 자료형 쓰려면 stdbool 헤더포함

int main() {
  int num[9], flag = 1;

  for (int i = 0; i < 9; i++)
    scanf("%d", &num[i]);

  int max = num[0];

  for (int i = 1; i < 9; i++)
    if (max < num[i]) {
      max = num[i];
      flag = i + 1;
    }

  printf("%d\n%d", max, flag);

  return 0;
}


// 1546 평균 *0712
int main() {
  int size;
  scanf("%d", &size);
  int score[size];

  // 그 과목들의 성적 입력받고 최대값 M 찾음
  for (int i = 0; i < size; i++)
    scanf("%d", &score[i]);
  int M = score[0];

  for (int j = 1; j < size; j++)
    if (score[j] > M)
      M = score[j]; // 최대값

  // 모든 점수를 */M x 100 으로 수정
  float new_score[size];
  for (int i = 0; i < size; i++) {
    new_score[i] = (float)score[i] / M * 100;
  }

  // 새로운 평균 도출 => double => float
  float sum = 0, average;
  for (int i = 0; i < size; i++)
    sum += new_score[i];
  average = sum / size;

  printf("%f", average);
}


// 10828 스택
int main() {
  int cmd_count, sp = 0;
  char command[10];
  // push, pop, size, empty, top 명령 입력arr
  int stack[10000]; // 명령의 수 N (1 ≤ N ≤ 10,000)

  scanf("%d", &cmd_count);

  while (cmd_count != 0) {
    scanf("%s", command);
    // C에서는 문자열을 switch-case 문에 넣을 수 X

    // 명령 블록
    if (strcmp("push", command) == 0) {
      scanf("%d", &stack[sp]); // 차례대로 삽입
      sp += 1;
    } else if (strcmp("pop", command) == 0) {
      if (sp > 0) {
        printf("%d\n", stack[sp - 1]); // top == stack[sp-1]
        sp--;
      } else
        printf("-1\n");

    } else if (strcmp("size", command) == 0)
      printf("%d\n", sp);
    else if (strcmp("empty", command) == 0)
      if (sp == 0)
        printf("1\n");
      else
        printf("0\n");
    else if (strcmp("top", command) == 0) {
      if (sp == 0)
        printf("-1\n");
      else
        printf("%d\n", stack[sp - 1]); 
    }

    // 사이클 순환
    cmd_count -= 1;
  }
}



/* 10773번 제로(스택)
0을 부르면 가장 최근에 쓴 수를 지운다
모든 수를 받아 합을 얻는다
0일 때 지울 수 있는 것 보장 가능함
*/

int main() {
  int count, input, sum=0, sp = 0;
  int stack[1000000];

  scanf("%d", &count);

  for (int i = 0; i < count; i++) {
    scanf("%d", &input);
    if (input != 0) {
      stack[sp] = input; // 차례대로 삽입
      sp += 1;
    } else if (sp > 0)
      sp -= 1; // 위치를 바꾸는 것으로 재입력
  }

  for (int i = 0; i < sp; i++) {
    sum += stack[i];
  }

  printf("%d", sum);
}

// 10845번 큐 
#include <stdio.h>
#include <string.h>

void push(int tmp, int* queue, int* back){
  back++; // 참조 호출로 받아온 back 증가
  queue[*back] = tmp; // 증가한 위치에 값 삽입
}
void pop(int, int*){
  
}

int main() {
  int cmd_count, tmp;
  int front=0, back=0;
  char cmd[10]; 
  int queue[10000]; // FIFO
  // 명령의 수 N (1 ≤ N ≤ 10,000)

  scanf("%d", &cmd_count);
  
  for(int i=0; i<cmd_count; i++) {
  // C에서 문자열 받기
  // scanf("%s", str); => 공백종료&OV위험


  
    fgets(cmd, sizeof(cmd), stdin); // 표준, 위험 적음
    if (strcmp(cmd, "push")){
      scanf("%d", &tmp); 
      push(tmp, queue, &back);
     
    }
    
  }
  
}

// 18258 큐 2 (동적할당 추가)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 10000

// 자리 부족시 동적할당
void push(int tmp, int **queue, int *back, int *size) {
    if (*back >= *size) {
        int new_size = (*size) * 2;
        int *tmp_ = (int *)realloc(*queue, new_size * sizeof(int));
        *queue = tmp_;
        *size = new_size;
    }
    (*queue)[*back] = tmp;
    (*back)++;
}

void pop(int **queue, int *front, int back) {
  if (back - *front == 0)
    printf("-1\n");
  else {
    printf("%d\n", (*queue)[*front]);
    (*queue)[*front] = 0; // 값을 0으로 초기화
    *front += 1; // 위치 이동
  }
}

int main() {
  int cmd_count, tmp, size=SIZE;
  int front = 0, back = 0;
  char cmd[10];
  int *queue = (int *)malloc(size * sizeof(int));
  // 처음에는 SIZE 로 크기지정

  scanf("%d", &cmd_count);

  for (int i = 0; i < cmd_count; i++) {

    scanf("%s", cmd);

    if (strcmp(cmd, "push") == 0) {
      scanf("%d", &tmp);
      push(tmp, &queue, &back, &size);
    } else if (strcmp(cmd, "pop") == 0)
      pop(&queue, &front, back);
    else if (strcmp(cmd, "size") == 0)
        printf("%d\n", back - front);
    else if (strcmp(cmd, "empty") == 0) {
      if (back - front  == 0)
        printf("1\n");
      else
        printf("0\n");
    }

    else if (strcmp(cmd, "front") == 0) {
      if (back - front == 0)
        printf("-1\n");
      else
        printf("%d\n", queue[front]);
    } else if (strcmp(cmd, "back") == 0) {
      if (back - front == 0)
        printf("-1\n");
      else {
        if (back != 0)
          printf("%d\n", queue[back - 1]);
        else
          printf("%d\n", queue[SIZE - 1]);
      }
    }
  }
}


// 34056번 : 콘서트 (해결 중)
#include <stdio.h>
#include <stdlib.h>

/*
1~N 번 방음벽
i번 방음벽 = Di 만큼의 소음 흡수 가능
콘서트 위치 = c, c+1번 방음벽 사이
x 의 소음 콘서트가 c~c+1 사이에 열렸을 때
c번 방음벽이 흡수하는 소음은 min(Dc, x)
c흡수 못 하면 c-1번 방음벽으로 간다. (c+1 는 c+2)

어떤 방음벽이 흡수한 소음이 x면
보강후 k번 방음벽이 흡수할 수 있는 소음은, Dk+x
*흡수한 소음의 양만큼 방음벽을 보강

첫줄 입력 = 방음벽 수
두줄 입력 = 소음의 크기
셋째줄 작업 수 
1 c x=> c~c+1 사이 x 콘서트 방음벽 보강 (x 크기는 1~10^9)
2 c => 해당 방음벽의 소음 측정
*/

void concert(int** D,int c, int x){
  // c part
  while((*D)[c] < x) {  
    if(c > 1){
     (*D)[c] = 0; 
      c--;
    }
  }   
  if(((*D)[c] > x) (*D)[c] -= x;

  // c+1 part

}

int getcapa(){
  
}



int main(){
  int *D; // 방음벽이 흡수 가능한 소음
  int N, Q;
  int input, c, x;

  scanf("%d", &N);
  D = (int*)malloc(N*sizeof(int));
  for(int i=0; i<N; i++)
    scanf("%d", &D[i]);
  
  scanf("%d", &Q);
  for(int j=0; j<Q; j++){
    scanf("%d", &input);
    if(input == 1){
      scanf("%d %d",&c, &x);
      concert(&D, c-1, x); 
    }
    else if(input == 2){
      scanf("%d", &c);
      
    }
  }

  free(D);
  

}


// 1094 막대기 (살짝컨닝함)

#include <stdio.h>
int main() {

  int X, cnt = 0;
  int arr[7] = {64, 32, 16, 8, 4, 2, 1}; // 이부분만 컨닝
  int count[7] = {0};
  scanf("%d", &X);

  for (int i = 0; i < 7; i++) {

    if ((X / arr[i]) >= 1) {
      count[i] = X / arr[i];
      X = X % arr[i];
    }
  }

  for (int i = 0; i < 7; i++) {
    if (count[i] == 1)
      cnt++;
  }
  printf("%d", cnt);
}


// 34056 콘서트 : 다 구현해두고
// 오타검수 & 자료형 범위문제만 gpt 도움받음
// 1~10^9 범위의 정수 => long long 사용 : 64비트(10^18까지 커버)
// long long 은 lld 로 받는다.

/*
1~N 번 방음벽
i번 방음벽 = Di 만큼의 소음 흡수 가능
콘서트 위치 = c, c+1번 방음벽 사이
x 의 소음 콘서트가 c~c+1 사이에 열렸을 때
c번 방음벽이 흡수하는 소음은 min(Dc, x)
c흡수 못 하면 c-1번 방음벽으로 간다. (c+1 는 c+2)

어떤 방음벽이 흡수한 소음이 x면
보강후 k번 방음벽이 흡수할 수 있는 소음은, Dk+x
*흡수한 소음의 양만큼 방음벽을 보강

첫줄 입력 = 방음벽 수
두줄 입력 = 소음의 크기
셋째줄 작업 수
1 c x=> c~c+1 사이 x 콘서트 방음벽 보강 (x 크기는 1~10^9)
2 c => 해당 방음벽의 소음 측정
*/

// 1 1 2 * 1 1 에 5이면 => 2 2 4 * 2 2 (capa 초과..)

#include <stdio.h>
#include <stdlib.h>

void concert(long long **D, int c, long long x, int N) {
  long long cx = x, cpx = x;
  int lc = c, rc = c + 1;

  // c part
  while ((*D)[lc] < cx && lc > -1) {
    cx -= (*D)[lc];
    (*D)[lc] *= 2;
    lc--;
  }
  if ((*D)[lc] >= cx && lc > -1 && cx > 0) {
    (*D)[lc] += cx;
  };

  // c+1 part
  while ((*D)[rc] < cpx && rc < N) {
    cpx -= (*D)[rc];
    (*D)[rc] *= 2;
    rc++;
  }
  if ((*D)[rc] >= cpx && rc < N && cpx > 0) {
    (*D)[rc] += cpx;
  };
}

int main() {
  long long *D, x; // 방음벽이 흡수 가능한 소음
  int N, Q;
  int input, c;

  scanf("%d", &N); // N 입력

  D = (long long *)malloc(N * sizeof(long long));

  for (int i = 0; i < N; i++)
    scanf("%lld", &D[i]); // Di

  scanf("%d", &Q); // Q 입력

  for (int j = 0; j < Q; j++) {
    scanf("%d", &input);
    if (input == 1) {
      scanf("%d %lld", &c, &x);
      concert(&D, c - 1, x, N);
    }
    if (input == 2) {
      scanf("%d", &c);
      printf("%lld\n", D[c - 1]);
    }
  }

  free(D);
}

