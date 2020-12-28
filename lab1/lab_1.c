#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

void funcExit(){
	printf("Процесс номер: %d завершился\n", getpid());
	printf("Родительский процесс: : %d \n", getppid());
}

int main ()
{
	atexit(funcExit);
	pid_t pid = fork();

	switch(pid){
	case -1: 
		perror("fork");
		exit(1);
	case 0:
	
		printf("Это процесс-потомок, его PID: %d\n", getpid());
		printf("Это процесс-потомок, его PPID: %d\n", getppid());
		
		exit(0);
		
	default: 
		printf("Это процесс родитель, его PID: %d\n", getpid());
		printf("Это процесс родитель, его PPID: %d\n", getppid());
		printf("Ждем завершения потомка...\n");
		waitpid(pid, 0, 0)	
	
		exit(0);
	}
	return 0;
}