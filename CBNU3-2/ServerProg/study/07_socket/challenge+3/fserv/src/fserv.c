#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <endian.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include <signal.h>
#include <pthread.h>
#include <dirent.h>

#include "fserv.h"

#define MAX_THREADS 8

static pthread_t threads[MAX_THREADS] ;
static int threads_isEmpty[MAX_THREADS] ; // 1=빔, 0=점유
static int n_threads = 0 ;
static int n_connections = 0;
// 정상 종료된 스레드가 n_threads--, isEmpty=0 해줘야
// thread들이 pool에서 동적으로 일을 잡을 수 있음.

typedef struct {
    int fda ;
    int index ;		
} thread_info_t ;

pthread_mutex_t thread_lock;

// int conn_fd 추가 (handler 함수 3종 모두)
int handle_list_request (int conn_fd, request_t * request) {
    // send response
    
    // ==== loop 1 count filepath_len ==== //
    DIR *dir = opendir(request->filepath); 
    if (dir == NULL) {
        perror("opendir error");
        send_response_header(conn_fd, 100, 0, NULL);
        return 1;
    }
    struct dirent *entry;
    uint64_t data_len = 0;
    while((entry = readdir(dir)) != NULL) {
        data_len += (strlen(entry->d_name) + 1);
        // \0 를 위해 +1B 추가한 길이로 계산
        // file1\0file2\0 ...
    } closedir(dir);


    // ==== loop 2 strcat, send response (payload-Data) ==== //
    char * data = malloc(data_len + 1);  // filepath : \0 구분 seq
    dir = opendir(request->filepath); 
    if (dir == NULL) {
        perror("opendir error");
        send_response_header(conn_fd, 100, 0, NULL);
        free(data);
        return 1;
    }
    int index = 0;
    while((entry = readdir(dir)) != NULL) {
        size_t len = strlen(entry->d_name);
        memcpy(data + index , entry->d_name, len);
        index += len; 
        data[index] = '\0'; // file1\0file2\0 ...
        index ++;
    } 
    closedir(dir);
    printf("data_len: %ld\n", data_len);

    send_response_header(conn_fd, 0, data_len, data);
    free(data); 
    return 0 ; //success
}

int handle_get_request (int conn_fd, request_t * request) {
    // send response

    // (1) data_len (file contents length) extract
    struct stat s;
    if (stat(request->filepath, &s) < 0) {
        perror("stat error");
        send_response_header(conn_fd, FAILURE, 0, NULL);
        return -1;
    }
    uint64_t data_len = (uint64_t)s.st_size;
    printf("file data length = %ld\n", data_len);
    
    // (2) data (file contents) extract
    char * data = malloc(data_len + 1);
    if(data == NULL) {
        perror("malloc error");
        send_response_header(conn_fd, FAILURE, 0, NULL);
        free(data);
        return -1;
    }
    FILE * fp = fopen(request->filepath, "rb");
    if(fp == NULL) {
        perror("malloc error");
        send_response_header(conn_fd, FAILURE, 0, NULL);
        free(data);
        return -1;
    }
    fread(data, 1, data_len, fp);
    data[data_len] = '\0'; 
    fclose(fp);

    send_response_header(conn_fd, SUCCESS, data_len, data);
    free(data);
	return 0 ; //success
}

int handle_put_request (int conn_fd, request_t * request) {
    // recv payload and write to the file
    // send response
    
    // receive client's request - only data (file contents)
    // (header handler : recv_request_header)
	uint64_t data_len = 0; 
    recv_bytes(conn_fd, (char *)&data_len, 8);
    uint64_t recv_data_len = be64toh(data_len);

    char * data = malloc(recv_data_len + 1);
    data[recv_data_len] = '\0';
    recv_bytes(conn_fd, data, recv_data_len);

	// write received file to server dir(req->filepath)
	FILE *fp = fopen(request->filepath, "wb");
    if (fp == NULL) {
        perror("fopen error");
        send_response_header(conn_fd, FAILURE, 0, NULL);
        free(data);
        return -1;
    }

    fwrite(data, 1, recv_data_len, fp);
	fclose(fp);

    send_response_header(conn_fd, SUCCESS, recv_data_len, NULL);

    free(data);
    return 0 ; //success
}

// receive req: type, filepath_len, filepath 
int recv_request_header (int conn_fd, request_t * request) {

    if (recv_bytes(conn_fd, (char *) &(request->type), 2) < 2) {
        return 1 ; //error
    }
    request->type = ntohs(request->type) ;
    
    if (recv_bytes(conn_fd, (char *) &(request->filepath_len), 2) < 2) {
        return 1 ; //error
    }
    request->filepath_len = ntohs(request->filepath_len) ;
    request->filepath = NULL ;
    
    if ((request->filepath = malloc(request->filepath_len + 1)) == NULL) {
        return 1 ; //error
    }

    if (recv_bytes(conn_fd, request->filepath, request->filepath_len) < request->filepath_len) {
        return 1 ; //error
    }

    request->filepath[request->filepath_len] = '\0' ;        
    return 0 ; //success
}

// thread runner function (void* void*)
// accept에서 분기해 나온 fda 가 arg에 들어감
// => recv_request_header 분기 (type에 따라 req 변경)
void * handle_connection (void * arg) {
    // int conn_fd = *((int *)arg) ;

    thread_info_t * thread_info = (thread_info_t *) arg;
    int conn_fd = thread_info->fda;
    int index = thread_info->index;
    free(arg) ;

    request_t * request = NULL ;
    request = calloc(1, sizeof(request_t)) ;

    do {
        if (recv_request_header(conn_fd, request)) {
            fprintf(stderr, "Error: failed to recv request header\n") ;
            break ;
        }

        if (request->type == LIST) {
            if (handle_list_request(conn_fd, request)) {
                fprintf(stderr, "Error: LIST request failed\n") ;
            }
            break ;
        }
        if (request->type == GET) {
            if (handle_get_request(conn_fd, request)) {
                fprintf(stderr, "Error: GET request failed\n") ;
            }
            break ;
        }
        if (request->type == PUT) {
            if (handle_put_request(conn_fd, request)) {
                fprintf(stderr, "Error: PUT request failed\n") ;
            }
            break ;
        }
        fprintf(stderr, "Error: unknown request type: %d\n", request->type) ;
        break ;        
    } while(0) ;
    // 예외처리하다 break 하면 바깥으로 가서 while(0)+break 가능

    if (request->filepath) free(request->filepath) ;    
    if (request) free(request) ;    
    close(conn_fd) ;

    pthread_mutex_lock(&thread_lock);
    threads_isEmpty[index] = 1;
    pthread_mutex_unlock(&thread_lock);

    return NULL ;
}

// int => void...
void sig_handler (int signum) {

    if(signum == SIGINT) {
        printf("\n Ctrl C detected \n");

        for(int i=0; i<MAX_THREADS; i++)
            if(threads_isEmpty[i] == 0) {
                pthread_join(threads[i], NULL);
            }
    }

    exit(0);
}

int main (int argc, char ** argv) 
{
    if(argc != 2) {
        printf("usage: ./fserv <port-number>\n");
        return 1;
    }

    // default value : option 주는 경우에 다르게
    char * host = strdup("127.0.0.1") ;
    int port = 0 ; 
    char * work_dir = strdup(".") ;
    signal(SIGINT, sig_handler); // SIGHAND

    // ==== parsing ==== //
    int opt ;
    while ((opt = getopt(argc, argv, "p:h:w:")) != -1) {
        switch (opt) {
            case 'p': 
                port = atoi(optarg) ;
                break;            
            case 'h':
                free(host) ;
                host = strdup(optarg) ;
                break;
            case 'w':
                free(work_dir) ;
                work_dir = strdup(optarg) ;
                break;
            default:
                fprintf(stderr, "Unknown option: %s\n", optarg) ;
                break;
        }
    }

    struct stat st ; 
    if (stat(work_dir, &st) < 0 || S_ISDIR(st.st_mode) == 0) {
        fprintf(stderr, "Error: invalid working directory: %s\n", work_dir) ;
        return EXIT_FAILURE;
    } // work_dir 존재하는지 확인
    
    memset(threads, 0, sizeof(threads)) ;
    // threads == multitherad 용

    // ==== connection establish ==== //
    int fd = socket(AF_INET, SOCK_STREAM, 0) ;

	struct sockaddr_in addr ;
	addr.sin_family = AF_INET ;
	addr.sin_port = htons(atoi(argv[1])) ;

	if (inet_aton("127.0.0.1", &(addr.sin_addr)) == 0) {
		fprintf(stderr, "inet_aton(127.0.0.1, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("bind() fails") ;
		exit(EXIT_FAILURE) ;
	}
	if (listen(fd, 16) == -1) {
		perror("listen(fd,16) fails") ;
		exit(EXIT_FAILURE) ;
	}

    // ==== multithreading ==== //
    pthread_mutex_init(&thread_lock, NULL);
    
    for(int i=0; i<MAX_THREADS; i++)
        threads_isEmpty[i] = 1;

    int i=0;
    while(1){
        thread_info_t * thread_info = malloc(sizeof(thread_info_t));
        thread_info->fda = accept(fd, (struct sockaddr *) NULL, NULL) ;
  
        pthread_mutex_lock(&thread_lock);
        for(int j=0; j<MAX_THREADS; j++){
            if(threads_isEmpty[j] == 1) {
                thread_info->index = j;
                pthread_create(&threads[j], NULL, handle_connection, (void *)thread_info);
                printf("\ndebug thread idx: %d\n", j);
                threads_isEmpty[j] = 0;
                break;
            }
        }
        pthread_mutex_unlock(&thread_lock);
        i++;
        n_threads++;
    }

    for(int i=0; i<MAX_THREADS; i++)
        pthread_join(threads[i], NULL);


    if (host != NULL)  free(host) ;
    if (work_dir != NULL) free(work_dir) ;

    return EXIT_SUCCESS ;
}
