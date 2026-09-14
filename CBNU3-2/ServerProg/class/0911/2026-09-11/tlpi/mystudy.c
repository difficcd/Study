#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 스레드가 실행할 함수
void *my_thread_func(void *arg) {
    char *msg = (char *)arg;
    
    // 1. pthread_self()로 자신의 스레드 ID 확인
    printf("[%ld] 스레드 시작: %s\n", (long)pthread_self(), msg);

    // 반환할 데이터를 힙 메모리에 할당
    long int *result = malloc(sizeof(long int));
    *result = strlen(msg);

    // 2. pthread_exit()로 자진 종료 및 결과 주소 전달
    pthread_exit((void *)result);
}

int main() {
    pthread_t thread_id;
    void *retval; // 스레드의 반환 주소를 받아올 포인터 변수

    char *input_msg = strdup("Hello POSIX Thread!");

    // 3. pthread_create()로 스레드 생성
    if (pthread_create(&thread_id, NULL, my_thread_func, input_msg) != 0) {
        perror("Thread creation failed");
        return 1;
    }

    printf("메인 스레드가 자식 스레드(%ld)의 종료를 기다립니다...\n", (long)thread_id);

    // 4. pthread_join()으로 자식 스레드 종료 대기 및 결과(retval) 수령
    if (pthread_join(thread_id, &retval) != 0) {
        perror("Thread join failed");
        return 1;
    }

    // 전달받은 힙 메모리의 데이터 사용 후 해제
    long int length = *(long int *)retval;
    printf("자식 스레드가 반환한 문자열 길이: %ld\n", length);
    free(retval);

    return 0;
}


