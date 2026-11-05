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

// fserv 처럼 recv_response_header 구현 해두면 좋음
// 가변적이어서 type 별로 나눠줘야함


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

    uint64_t data_len=0;
    char * data = NULL;
    recv_bytes(conn_fd, (char *) &response.response_status, 2);
    if(ntohs(response.response_status) == FAILURE){
        printf("server response error\n");
        return 1;
    }
    recv_bytes(conn_fd, (char *) &data_len, 8);

    data_len = be64toh(data_len);
    data = malloc((size_t)data_len + 1);
    recv_bytes(conn_fd, data, (size_t) data_len);


    printf("\n ==== file list ==== \n");
    data[data_len] = '\0'; 
    char * p = data;
    while (p < data + data_len && *p != '\0') {
        if (*p != '.')        
            printf("%s\n", p);
    
        p += strlen(p) + 1;        
    } printf("\n");

    return 0 ;
}

int handle_get_command (int conn_fd, char * src, char * dst) 
{
    // get src dst -> src 가 다운로드 대상 파일, dst 가 가져올 그릇 파일
    // 다운받을 파일 src 를 request 로 포장해서 보내줘야 함. (specified path)
    
    // ==== request header ==== //
    request_t request;
    request.type = GET;
    request.filepath_len = (uint16_t) (strlen(src));
    request.filepath = src; // argv[3]

    printf("type: %d, filepath_len: %d, filepath: %s \n", 
            request.type, request.filepath_len, dst);
    send_request_header(conn_fd, request.type, 
                        request.filepath_len, request.filepath, 0, NULL); 
                        

    // response display
    response_t response;
    response.response_status = FAILURE;
    // response.filepath_len = 0;
    // response.filepath = NULL;

    recv_bytes(conn_fd, (char *) &response.response_status, 2);
    if(ntohs(response.response_status) == FAILURE){
        printf("server response error\n");
        return 1;
    }

    uint64_t data_len = 0; 
    recv_bytes(conn_fd, (char *)&data_len, 8);
    uint64_t recv_data_len = be64toh(data_len);

    char * data = malloc(recv_data_len + 1);
    data[recv_data_len] = '\0';
    recv_bytes(conn_fd, data, recv_data_len);

	// write received file to client dir(dst)
	FILE *fp = fopen(dst, "wb");
    if(fp == NULL) {
        perror("fopen error");
        return -1;
    }
    
    fwrite(data, 1, recv_data_len, fp);
	fclose(fp);

    return 0 ;
}

int handle_put_command (int conn_fd, char * src, char * dst) 
{
    // put src dst -> src 의 파일 내용 추출해서 서버의 dst 파일로 보내기
    // 보내는 경로인 dst 를 request 로 포장해서 보내줘야 함. (specified path)
    // file data(src로부터 추출), file path(dst, server에서 만들 파일경로/이름) 

    // ==== request header ==== //
    request_t request;
    request.type = PUT;
    request.filepath_len = (uint16_t) strlen(dst);
    request.filepath = dst; // argv[4]

    // (1) data_len (file contents length) extract
    struct stat s;
    if (stat(src, &s) < 0) {
        perror("stat error");
        return -1;
    }
    uint64_t data_len = (uint64_t) s.st_size;
    printf("file data length = %ld\n", data_len);
    
    // (2) data (file contents) extract
    char * data = malloc(data_len + 1);
    FILE * fp = fopen(src, "rb");
    fread(data, 1, data_len, fp);
    data[data_len] = '\0'; 
    fclose(fp);

    printf("type: %d, filepath_len: %d, filepath: %s \n", 
            request.type, request.filepath_len, dst);
    send_request_header(conn_fd, request.type, 
                        request.filepath_len, request.filepath,
                        data_len, data); // 0, NULL => (1), (2)
    

    // response display
    response_t response;
    response.response_status = FAILURE;
    // response.filepath_len = 0;
    // response.filepath = NULL;

    uint64_t recv_data_len = 0;
    recv_bytes(conn_fd, (char *) &response.response_status, 2);
    if(ntohs(response.response_status) == FAILURE){
        printf("server response error\n");
        return 1;
    }
    
    recv_bytes(conn_fd, (char *) &recv_data_len, 8);
    recv_data_len = be64toh(recv_data_len);
    printf("server response : received %ld bytes (upload file)\n", recv_data_len);
    
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
