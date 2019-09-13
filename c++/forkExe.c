#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
int main(){
	pid_t pid;
	//fork a child process
	pid = fork();
	struct timeval* tv_sec, t_usec;
	int fd;
	char *ptr;
	fd = shm_open()
	if(pid < 0 ){
		fprintf(stderr, "%s\n", "Fork failed");
		return 1;
	}
	else if( pid == 0){
		gettimeofday(&tv_sec, NULL);
		execlp("hm/Documents","ls", NULL);
	}
	else{
		wait(NULL);
		gettimeofday(&t_usec, NULL);
		printf("%s\n", "child complete");
	}

	return 0;
}