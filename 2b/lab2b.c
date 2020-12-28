#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define FIFO_PATH "fifo0001"


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
 
            puts("HI, STDOUT!!!");

            int fifo_child = open(FIFO_PATH, O_RDONLY);
            if (fifo_child == -1) {
                fprintf(stderr, "Невозможно открыть FIFO\n");
                exit(EXIT_FAILURE);
            }

            long int child_buffer[2];
            if (read(fifo_child, &child_buffer[0], sizeof(long int)) == -1 || read(fifo_child, &child_buffer[1], sizeof(long int)) == -1) {
                fprintf(stderr, "Невозможно считать с FIFO\n");
                exit(EXIT_FAILURE);
            }

            sleep(5);
            time_t child_time = time(NULL);
            printf("\nВремя: %s", ctime(&child_time));

            printf("Родительское время: %s", ctime(&child_buffer[0]));
            printf("\nРодительский PID: %lu\n", child_buffer[1]);

            close(fifo_child);
            exit(0);
        default:

            int fifo_parent = open(FIFO_PATH, O_WRONLY);
            if (fifo_parent == -1) {
                fprintf(stderr, "Невозможно открыть FIFO\n");
                exit(EXIT_FAILURE);
            }

            long int parent_buffer[2];
            parent_buffer[0] = time(NULL);
            parent_buffer[1] = getpid();

            write(fifo_parent, parent_buffer, sizeof parent_buffer);
            close(fifo_parent);

            if (wait(0) == -1) {
                perror("WAIT ошибка");
                exit(EXIT_FAILURE);
            }
    }

    return 0;
}