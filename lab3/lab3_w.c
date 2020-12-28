#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#define FILE_PATH "lab3_r.c" 
#define IPC_ID 45

typedef struct Value {
    char str[200];
    pid_t pid;
} value;

int shm_id;

void memory_destructor(int signal){
    printf("Я ДЕСТРУКТОР!\n");
    struct shmid_ds *buf = 0;
    shmctl(shm_id, IPC_RMID, buf);

    exit(0);
}


int main(void) {
    signal(SIGINT, memory_destructor);

    key_t ipc_key = ftok(FILE_PATH, IPC_ID);
    if (ipc_key == -1) {
        printf("ОШИБКА, KEY: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    shm_id = shmget(ipc_key, sizeof(value), 0666 | IPC_CREAT);
    if (shm_id == -1) {
        printf("ОШИБКА, SHM: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    value *forward_value; 

    forward_value = (value *) shmat(shm_id, NULL, 0);
    if (forward_value == (value *) -1) {
        printf("Ошибка, SHMAT: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if(forward_value->pid != 0){
        printf("Процесс уже запущен!!!!\n");
        exit(0);
    }

    while (true) {
        value tmp_value;

        time_t t = time(NULL);
        strncpy((char *) tmp_value.str, ctime(&t), 199);
        tmp_value.str[199] = '\0';

        tmp_value.pid = getpid();

        printf("Данные отправлены!\n");
        *forward_value = tmp_value;

        sleep(2); 
    }
}