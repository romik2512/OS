#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>

#define IPC 77
#define FILE_PATH "lab4_r.c"
#define SEMAPHORE_NAME "SEMAPHORE_1"

int shmID;

struct sembuf semsignal = { 0, 1, SEM_UNDO };
struct sembuf semwait = { 0, -1, SEM_UNDO };

typedef struct Value {
	char str[100];
	pid_t pid;
} value;

void destructor(int signal){
	struct shmid_ds *buffer = 0;
	shmctl(shmID, IPC_RMID, buffer);
	exit(0);
}

int main(){
	
	signal(SIGINT, destructor);
	
	key_t key = ftok(FILE_PATH, IPC);
	if (key == -1) {
        printf("KEY ОШИБКА: %d\n", errno);
        exit(EXIT_FAILURE);
    }
	
	int shmID = shmget(key, sizeof(value), 0666 | IPC_CREAT);
	if(shmID == -1){
		printf("Ошибка SHM: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	value *forward_value = (value *) shmat(shmID, NULL, 0);
	if (forward_value == (value *) - 1){
		printf("Ошибка SHMAT: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	if(forward_value->pid != 0){
		printf("Процесс уже запущен!\n");
		exit(0);
	}
	
	key_t semkey = ftok(SEMAPHORE_NAME, IPC);
	
	int semID = semget(semkey, 1, 0666 | IPC_CREAT);
	if(semID == -1){
		printf("SEMGET ОШИБКА: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	while(1){
		
		value tmp_value;
		
		time_t local_time = time(NULL);
		
		int semSigRes = semop(semID, &semsignal, 1);
		if(semSigRes == -1){
			printf("Ошибка открытия SEMAPHORE\n");
			exit(EXIT_FAILURE);
		}	
		
		tmp_value.pid = getpid();
		strncpy((char *) tmp_value.str, ctime(&local_time), 99);
		tmp_value.str[99] = '\0';
		
		printf("Данные отправлены!\n");
		*forward_value = tmp_value;
		
		int semwaitRes = semop(semID, &semwait, 1);
		if(semwaitRes == -1){
			printf("Ошибка закрытия SEMAPHORE\n");
			exit(EXIT_FAILURE);
		}
				
		sleep(3);
	}
	
}