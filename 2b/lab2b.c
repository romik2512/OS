#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define FIFO_PATH "fifo0001"


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
    unlink(FIFO_PATH);

    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("FIFO ошибка");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
	
    switch (pid) {
		
        case -1:            
            perror("Fork ошибка");
            exit(EXIT_FAILURE);
			
        case 0:
 
            int fifo_child = open(FIFO_PATH, O_RDONLY);
            if (fifo_child == -1) {
                fprintf(stderr, "Невозможно открыть FIFO\n");
                exit(EXIT_FAILURE);
            }
										
			char buffer[1024];
			read(fifo_child, &buffer, sizeof(buffer));
			printf("%s", buffer);
	       	close(fifo_child);	
		
			long int chTime = time(NULL);
			printf("Текущее время в дочернем процессе: %s", ctime(&chTime));
		
			sleep(5);
				
        default:

            int fifo_parent = open(FIFO_PATH, O_WRONLY);
            if (fifo_parent == -1) {
                fprintf(stderr, "Невозможно открыть FIFO\n");
                exit(EXIT_FAILURE);
            }
			
			long int pTime = time(NULL);
			int pid = getpid();

			char *str = "Текущее время в родительском процессе: ";
			str = concat(str, ctime(&pTime));
			str = concat(str, " PID родительского процесса: ");
			str = concat(str, toChar(pid));
			str = concat(str, "\n");

			write(fifo_parent, (void *) str, strlen(str) + 1);
			close(fifo_parent);
			
			if (waitpid(pid, 0, 0) == -1) {
                perror("WAIT Ошибка");
                exit(EXIT_FAILURE);
            }
			
			
    }

    return 0;
}