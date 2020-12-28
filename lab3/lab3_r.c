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

void memory_destructor(int signal)
{
    printf("\nЯ ДЕСТРУКТОР!\n");
    exit(0);
}

int main(void) {
    signal(SIGINT, memory_destructor);

    key_t ipc_key = ftok(FILE_PATH, IPC_ID);
    if (ipc_key == -1) {
        printf("ОШИБКА, KEY: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    shm_id = shmget(ipc_key, sizeof(value), 0666 | IPC_EXCL);
    if (shm_id == -1) {
        printf("ОШИБКА, SHM: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    value *received_value = (value *) shmat(shm_id, NULL, 0);
    if (received_value == (value *) -1) {
        printf("Ошибка, SHMAT: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    while (true) {
        value local_value;

        time_t local_time = time(NULL);
        strncpy((char *) local_value.str, ctime(&local_time), 199);
        local_value.str[199] = '\0';
        local_value.pid = getpid();


        printf("PID: %d, ВРЕМЯ: %s \n", local_value.pid, local_value.str);
        printf("Полученные PID: %d, ВРЕМЯ: %s\n", received_value->pid, received_value->str);

        sleep(2);
    }
}