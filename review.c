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
// flag = 0 으로 설정해서 오답이 있었음 => 로직을 섬세하게 짜서 미연에 오류 방지include <stdio.h>


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

// 10845번 큐 : 독서실 가기전에 잠깐 연습
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








