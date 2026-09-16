
# include <stdio.h>
#include <unistd.h>

void main(){

  printf("Hellow World\n");
  
  int i=0;   
  pid_t my_pid = getpid();
  pid_t my_ppid = getppid();

  while(1){
	i++;
  	printf("ppid:%d,   pid: %d, loop: %d \n", my_ppid, my_pid ,i);
  }
  
}


