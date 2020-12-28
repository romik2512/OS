#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

#define SIZE_OF_ARR 20
#define NUMBER_OF_THREADS 10

int* arr;
int counter=0;


void* readT(){
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);
		printf("ID потока: %ld, Размер массива: %d, Счетчик: %d\n", (long)pthread_self(), arr[counter], counter);
		fflush(stdout);
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);
		sleep(1);
	}

}

void* writeT(){
	while(1){
		pthread_mutex_lock(&mutex);
		counter++;
		if(counter < SIZE_OF_ARR){
			arr[counter] = counter;
		}
		int random = rand() %10;
		if(random < 5) {
			pthread_cond_broadcast(&cond);
		} else {
		pthread_cond_signal(&cond);
		}
		
		pthread_mutex_unlock(&mutex);
		sleep(rand() % 10);
	}

}

int main(){
	arr = (int*)calloc(SIZE_OF_ARR, sizeof(int));
	pthread_t threads[NUMBER_OF_THREADS];
	
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	
	int i;
	
	pthread_create(&threads[0], NULL, writeT, NULL);
	
	for(i = 0; i < NUMBER_OF_THREADS; i++){
		pthread_create(&threads[i], NULL, readT, NULL);
	}

	for(i = 0; i < NUMBER_OF_THREADS; i++){
		pthread_join(threads[i], NULL);
	}
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	free(arr);
	return 0;

}