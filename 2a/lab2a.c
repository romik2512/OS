#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

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

            close(for_pipe[1]); 

            sleep(5);
            long int child_buffer[2];

            time_t child_time = time(NULL);
            printf("Время: %s", ctime(&child_time));

            read(for_pipe[0], &child_buffer[0], sizeof(long int));
            printf("Родительское время: %s", ctime(&child_buffer[0]));

            read(for_pipe[0], &child_buffer[1], sizeof(long int));
            printf("\nРодительский PID: %lu\n", child_buffer[1]);

            close(for_pipe[0]);
            exit(0);
        default:

            close(for_pipe[0]); 

            long int parent_buffer[2];
			
            parent_buffer[0] = time(NULL);
            parent_buffer[1] = getpid();

            write(for_pipe[1], parent_buffer, sizeof parent_buffer);
            close(for_pipe[1]);

            if (wait(0) == -1) {
                perror("Wait ошибка");
                exit(EXIT_FAILURE);
            }
    }

    return 0;
}