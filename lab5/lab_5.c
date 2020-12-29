#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;

#define SIZE_OF_ARR 20
#define NUMBER_OF_THREADS 10

int arr[SIZE_OF_ARR];
int counter=0;
int check=0;
void* readT(){
	while(1){
		pthread_mutex_lock(&mutex);
		if(check ==0){
		printf("ID потока: %ld, Значение ячейки: %d, Счетчик: %d\n", (long)pthread_self(), arr[counter], counter);
		fflush(stdout);
		}
        pthread_mutex_unlock(&mutex); 
        if (counter == SIZE_OF_ARR){
            check=1;
            pthread_exit(0); 
        }
        pthread_mutex_lock(&mutex);
	}
}

void* writeT(){
	while(counter < SIZE_OF_ARR){
		pthread_mutex_unlock(&mutex);
		pthread_mutex_lock(&mutex);
		counter++;
		arr[counter] = counter;
		pthread_mutex_unlock(&mutex);
		sleep((rand() % 3)+1);
	}
	pthread_exit(0);
}

int main(){
	pthread_t threads[NUMBER_OF_THREADS];

	int i;
	
	pthread_create(&threads[0], NULL, writeT, NULL);
	
	for(i = 0; i < NUMBER_OF_THREADS; i++)
		pthread_create(&threads[i], NULL, readT, NULL);
	
	for(i = 0; i < NUMBER_OF_THREADS; i++)
		pthread_join(threads[i], NULL);
	
	pthread_mutex_destroy(&mutex);

	return 0;

}