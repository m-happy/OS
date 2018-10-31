#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main(){
	pid_t pid;
	//fork a child process
	pid = fork();
	if(pid < 0 ){
		fprintf(stderr, "%s\n", "Fork failed");
		return 1;
	}
	else if( pid == 0){
		execlp("hm/Documents","ls", NULL);
	}
	else{
		wait(NULL);
		printf("%s\n", "child complete");
	}
	return 0;
}