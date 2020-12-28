#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_rwlock_t rwlock;

#define SIZE_OF_ARR 20
#define NUMBER_OF_THREADS 10

int* arr;
int counter=0;


void* readT(){
	while(1){
		pthread_rwlock_wrlock(&rwlock);
		printf("ID потока: %ld, Размер массива: %d, Счетчик: %d\n", (long)pthread_self(), arr[counter], counter);
		fflush(stdout);
		pthread_rwlock_unlock(&rwlock);
		sleep(1);
	}

}

void* writeT(){
	while(1){
		pthread_rwlock_rdlock(&rwlock);
		counter++;
		if(counter < SIZE_OF_ARR){
			arr[counter] = counter;
		}
		pthread_rwlock_unlock(&rwlock);
		sleep(1);
	}

}

int main(){
	arr = (int*)calloc(SIZE_OF_ARR, sizeof(int));
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

	free(arr);
	return 0;

}