#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <endian.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "fserv.h"

int handle_list_command (int conn_fd, char * dir) 
{
    // ==== request header ==== //
    request_t request;
    request.type = LIST;
    request.filepath_len = strlen(dir);
    request.filepath = dir; // argv[3]

    printf("type: %d, len: %ld, filepath: %s\n", LIST, strlen(dir), dir);
    send_request_header(conn_fd, request.type, 
                        request.filepath_len, request.filepath, 0, NULL);
    

    // ==== response display: payload(data) -> filenames ==== //
    response_t response;
    response.response_status = FAILURE;
    // response.filepath_len = 0;
    // response.filepath = NULL;

    uint16_t data_len=0;
    char * data = NULL;
    recv_bytes(conn_fd, (char *) &response.response_status, 2);
    if(ntohs(response.response_status) == FAILURE){
        printf("server response error\n");
        return 1;
    }
    recv_bytes(conn_fd, (char *) &data_len, 2);

    data_len = ntohs(data_len);
    data = malloc((size_t)data_len + 1);
    recv_bytes(conn_fd, data, (size_t) data_len);


    printf("\n ==== file list ==== \n");
    data[data_len] = '\0'; 
    for (size_t i = 0; i < data_len; i++) {
        if (data[i] == '\0') 
            data[i] = '\n'; 
    }
    printf("%s\n", data); 

    return 0 ;
}

int handle_get_command (int conn_fd, char * src, char * dst) 
{
    return 0 ;
}

int handle_put_command (int conn_fd, char * src, char * dst) 
{
    // 우선 src, dst 는 경로정보 없는 파일 이름인 것으로 가정하고 구현

    // put src dst -> src 의 파일 내용 추출해서 서버의 dst 파일로 보내기
    // 보내는 경로인 dst 를 request 로 포장해서 보내줘야 함. (specified path)
    // file data(src로부터 추출), file path(dst, server에서 만들 파일경로/이름) 

    // ==== request header ==== //
    request_t request;
    request.type = PUT;
    request.filepath_len = strlen(dst);
    request.filepath = dst; // argv[4]

    // 여기서 data 추출해서 req_header의 0, NULL을 채워줘야 함.
    // lib 으로 파일 길이 구해서 0 자리에 넣기(1)
    // 파일 길이에 따라 버퍼에 저장해서 NULL 자리에 데이터 char*로 넣기 (2)

    struct stat s;
    if (stat(src, &s) < 0) {
        perror("stat error");
        return -1;
    }
    uint16_t data_len = (uint16_t)s.st_size;
    printf("file data length = %d\n", data_len);
    

    printf("type: %d, len: %ld, filepath: %s\n", request.type, strlen(dir), dir);
    send_request_header(conn_fd, request.type, 
                        request.filepath_len, request.filepath, 0, NULL);
    
/*
    // ==== response display: payload(data) -> file contents ==== //
    response_t response;
    response.response_status = FAILURE;
    // response.filepath_len = 0;
    // response.filepath = NULL;

    uint16_t data_len=0;
    char * data = NULL;
    recv_bytes(conn_fd, (char *) &response.response_status, 2);
    if(ntohs(response.response_status) == FAILURE){
        printf("server response error\n");
        return 1;
    }
    recv_bytes(conn_fd, (char *) &data_len, 2);

    data_len = ntohs(data_len);
    data = malloc((size_t)data_len + 1);
    recv_bytes(conn_fd, data, (size_t) data_len);


    printf("\n ==== file list ==== \n");
    data[data_len] = '\0'; 
    for (size_t i = 0; i < data_len; i++) {
        if (data[i] == '\0') 
            data[i] = '\n'; 
    }
    printf("%s\n", data); 
    */
    
    return 0 ;
}

int main (int argc, char ** argv) 
{
    char * ip_port = NULL, * ip = NULL, * port = NULL ;
    int portnum ;

    char * command = NULL ;
    char * dir = NULL, * src = NULL, * dst = NULL ;

    if (argc < 4 || 5 < argc) {
        printf("./fsh <ip>:<portnum> <command> [args...]\n");
        fprintf(stderr, "Error: Invalid Arguments\n") ;
        return EXIT_FAILURE ;
    }

    // parsing
    ip_port = strdup(argv[1]) ;    
    ip = strtok(ip_port, ":") ;
    port = strtok(NULL, ":") ;
    portnum = atoi(port) ;

    if (ip == NULL || port == NULL || strtok(NULL, ":") || 
	    portnum < 0 || 65535 < portnum ) {
        fprintf(stderr, "Error: IP address and port number are invalid\n") ;
        return EXIT_FAILURE ;
    } 

    if (strcmp(argv[2], "list") && strcmp(argv[2], "get") && strcmp(argv[2], "put") ) {
        fprintf(stderr, "Error: unknown command\n") ;
	    return EXIT_FAILURE ;
    }

    command = strdup(argv[2]) ;


    // ==== client : connection establish ==== //
	int fd = socket(AF_INET, SOCK_STREAM, 0) ;

	struct sockaddr_in srv_addr ;
	srv_addr.sin_family = AF_INET ;
	srv_addr.sin_port = htons(atoi(port)) ;

	if (inet_aton(ip , &(srv_addr.sin_addr)) == 0) {
		fprintf(stderr, "inet_aton(127.0.0.1, addr.sin_addr) returns zero\n") ;
		exit(EXIT_FAILURE) ;
	}
	if (connect(fd, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) == -1) {
		perror("connect() fails") ;
		exit(EXIT_FAILURE) ;
	}

    /*TODO*/
    enum REQUEST_TYPES type;

    do {
        if (strcmp(command, "list") == 0) {
            if(argc != 4){
                fprintf(stderr, "Error: Invalid Arguments\n") ;
                return EXIT_FAILURE;
            }
            type = LIST;
            char * dir = argv[3];
            handle_list_command(fd, dir);
	    break ;

        }
        if (strcmp(command, "get") == 0) {
            if(argc != 5){
                fprintf(stderr, "Error: Invalid Arguments\n") ;
                return EXIT_FAILURE;
            }
            type = GET;
            char * src = argv[3];
            char * dst = argv[4]; 
            handle_get_command(fd, src, dst);

	    break ;
        }

        if (strcmp(command, "put") == 0) {
            if(argc != 5){
                fprintf(stderr, "Error: Invalid Arguments\n") ;
                return EXIT_FAILURE;
            }
            type = PUT;
            char * src = argv[3];
            char * dst = argv[4]; 
            handle_put_command(fd, src, dst);
	    break ;
        }

        fprintf(stderr, "Error: unknown command\n") ;
        break ;
    } while (0) ; 

    /*TODO*/

    return EXIT_SUCCESS ;
}
