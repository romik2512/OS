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



int main(){
	
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
	
	value *recivedValue = (value *) shmat(shmID, NULL, 0);
	if (recivedValue == (value *) - 1){
		printf("Ошибка SHMAT: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	key_t semkey = ftok(SEMAPHORE_NAME, IPC);
	
	int semID = semget(semkey, 1, 0666 | IPC_CREAT);
	if(semID == -1){
		printf("SEMGET ОШИБКА: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	while(1){
		
		value myValue;
		
		time_t local_time = time(NULL);
		
		int semSigRes = semop(semID, &semsignal, 1);
		if(semSigRes == -1){
			printf("Ошибка открытия SEMAPHORE\n");
			exit(EXIT_FAILURE);
		}	
		
		myValue.pid = getpid();
		strncpy((char *) myValue.str, ctime(&local_time), 99);
        myValue.str[99] = '\0';
		
		
		printf("PID: %d, Время: %s \n", myValue.pid, myValue.str);
		printf("Полученные PID: %d, Время: %s \n", recivedValue->pid, recivedValue->str);
		
		int semSigWait = semop(semID, &semwait, 1);
		if(semSigWait == -1){
			printf("Ошибка открытия SEMAPHORE\n");
			exit(EXIT_FAILURE);
		}	

		sleep(1);
	}
	


}