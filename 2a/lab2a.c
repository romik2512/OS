#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

char* concat(const char *str1, const char *str2)
{
    char *result = malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

char* toChar(int num){
	char* ch;
	ch = (char*)malloc(10 * sizeof(char));
	int numOfDigits = 0;
	while(num > 9){
		ch[numOfDigits++] = (num % 10) + '0';
		num = num / 10;
	}
	ch[numOfDigits++] = num + '0';
	ch[numOfDigits] = '\0';
	char t;

	for (int i = 0; i < numOfDigits/2; i++){
		t = ch[i];
		ch[i] = ch[numOfDigits - 1 - i];
		ch[numOfDigits - 1 - i] = t;
	}
	numOfDigits = 0;
	return ch;

}

int main() {
	
    int for_pipe[2];  

    if (pipe(for_pipe) == -1) {
        perror("Pipe ошибка");
        exit(EXIT_FAILURE);
    }
	
    pid_t pid = fork();
	
    switch (pid) {
        case -1:
            perror("Fork ошибка");
            exit(EXIT_FAILURE); 
        case 0:
		
			long int chTime = time(NULL);
			char buffer[BUFFER_SIZE];
			int len;
			close(for_pipe[1]);
			while((len = read(for_pipe[0], buffer, BUFFER_SIZE)) != 0)
			write(2, buffer, len);
			close(for_pipe[0]);
			printf("Текущее время в дочернем процессе: %s", ctime(&chTime));
			sleep(5);
																			
        default:
          						
			long int pTime = time(NULL);
			int pid = getpid();

			char *str = "Текущее время в родительском процессе: ";
			str = concat(str, ctime(&pTime));
			str = concat(str, " PID родительского процесса: ");
			str = concat(str, toChar(pid));
			str = concat(str, "\n");
		
			close(for_pipe[0]);
			write(for_pipe[1], (void *) str, strlen(str) + 1);
			close(for_pipe[1]);
			
			if (waitpid(pid, 0, 0) == -1) {
                perror("WAIT Ошибка");
                exit(EXIT_FAILURE);
            }
			
    }

    return 0;
}