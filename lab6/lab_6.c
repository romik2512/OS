#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_rwlock_t rwlock;

#define SIZE_OF_ARR 20
#define NUMBER_OF_THREADS 10

int arr[SIZE_OF_ARR];
int counter=0;


void* readT(){
	while(1){
		pthread_rwlock_wrlock(&rwlock);
		printf("ID потока: %ld, Значение ячейки: %d, Счетчик: %d\n", (long)pthread_self(), arr[counter], counter);
		fflush(stdout);
		pthread_rwlock_unlock(&rwlock);
		if (counter == SIZE_OF_ARR)
            pthread_exit(0);
		sleep(1);
	}

}

void* writeT(){
	while(counter < SIZE_OF_ARR){
		pthread_rwlock_rdlock(&rwlock);
		counter++;
		arr[counter] = counter;
		pthread_rwlock_unlock(&rwlock);
		sleep(1);
	}
pthread_exit(0);
}

int main(){
	pthread_t threads[NUMBER_OF_THREADS];
	pthread_rwlock_init(&rwlock, NULL);
	
	int i;
	
	pthread_create(&threads[0], NULL, writeT, NULL);
	
	for(i = 0; i < NUMBER_OF_THREADS; i++){
		pthread_create(&threads[i], NULL, readT, NULL);
	}

	for(i = 0; i < NUMBER_OF_THREADS; i++){
		pthread_join(threads[i], NULL);
	}
	
	pthread_rwlock_destroy(&rwlock);

	return 0;

}