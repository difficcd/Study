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





