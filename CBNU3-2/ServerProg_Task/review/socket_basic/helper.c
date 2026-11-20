
/// helper review/prac (while send & recv)


// send helper
ssize_t send_bytes (int conn, char *buf, size_t len) 
{
    ssize_t current_len = 0, send_bytes = 0;
    while(current_len < len){
        send_bytes = send(conn, buf+current_len, len-current_len, 0);
        if(send_bytes <= 0) return -1;
        current_len += send_bytes;
    }
    return current_len;
}

// send helper
ssize_t send_bytes (int conn, char *buf, size_t len) 
{
    // 간단하게
    // 보낸 길이를 모아둘거 (전송한거의총량)랑 그때그때전송한거 temp_len 
    // while (전송한거 총량이 < 보내고싶은 arg len) 
    // send 에서는 시작버퍼주소가 buf+current_len(byte)
    // 길이는 len-current_len
    // 운좋으면 한번에 보내는거고 아님 끊어서 (Nagle) 보내고 함.

    int current_len = 0, sent_bytes = 0;
	while(current_len < len){
		sent_bytes = send(conn, buf+current_len, len-current_len, 0);
		if(sent_bytes <  0) return -1; 
        if(sent_bytes == 0) return 0;
        // -1이면 실패 0이면 통신 정상종료
		current_len += sent_bytes;
	}
	return current_len;
}

// recv helper 
ssize_t recv_bytes (int conn, char *buf, size_t len) 
{
    // 똑같은 로직이고 당연함
    int current_len = 0, received_bytes = 0;
	while(current_len < len){
		received_bytes = recv(conn, buf+current_len, len-current_len, 0) ;
		if(received_bytes == -1){
			perror("string receive: failed");
			return -1;
		}
		if(received_bytes == 0) {
			return 0;
		} 
		current_len += received_bytes;
	}
	return current_len;
}    

