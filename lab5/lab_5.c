#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;

#define SIZE_OF_ARR 20
#define NUMBER_OF_THREADS 10

int* arr;
int counter=0;


void* readT(){
	while(1){
		pthread_mutex_lock(&mutex);
		printf("ID потока: %ld, Размер массива: %d, Счетчик: %d\n", (long)pthread_self(), arr[counter], counter);
		fflush(stdout);
		pthread_mutex_unlock(&mutex);
		sleep(3);
	}

}

void* writeT(){
	while(1){
		pthread_mutex_lock(&mutex);
		counter++;
		if(counter < SIZE_OF_ARR){
			arr[counter] = counter;
		}
		pthread_mutex_unlock(&mutex);
		sleep(3);
	}

}

int main(){
	arr = (int*)calloc(SIZE_OF_ARR, sizeof(int));
	pthread_t threads[NUMBER_OF_THREADS];
	pthread_t writer_thread;
	
	int i;
	
	pthread_create(&writer_thread, NULL, writeT, NULL);
	
	for(i = 0; i < NUMBER_OF_THREADS; i++){
		pthread_create(&threads[i], NULL, readT, NULL);
	}

	for(i = 0; i < NUMBER_OF_THREADS; i++){
		pthread_join(threads[i], NULL);
	}
	
	pthread_join(writer_thread, NULL);
	
	pthread_mutex_destroy(&mutex);

	free(arr);
	return 0;

}