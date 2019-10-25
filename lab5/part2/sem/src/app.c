#include <stdio.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define ARR_LEN 26
static char arr[] = "abcdefghijklmnopqrstuvwxyz";
static sem_t sem;

typedef void* (*ThreadFunc)(void*);

void* InvertCase(void* arg){
	int (*func)(int);
	if (arr[0] >= 'a' && arr[0] <= 'z')
		func = toupper;
	else 
		func = tolower;
	for (size_t i = 0; i < ARR_LEN; i++){
		arr[i] = (char) func(arr[i]);
	}
	// unlock semaphore
	sem_post(&sem);
	return NULL;
}

void* InvertArray(void* arg){
	for (size_t i = 0; i < ARR_LEN/2; ++i){
		char temp = arr[i];
		arr[i] = arr[ARR_LEN-i-1];
		arr[ARR_LEN-i-1] = temp;
	}
	// unlock semaphore
	sem_post(&sem);
	return NULL;
}

int main(){
	// init semaphore
	sem_init(&sem, 0, 1);

	ThreadFunc threadFuncPtrs[] = {InvertCase, InvertArray};
	size_t numThreadFuncs = 2;
	size_t threadFuncIter = 0;
	while(1){
		ThreadFunc toStart = threadFuncPtrs[threadFuncIter];
		threadFuncIter = (threadFuncIter + 1) % numThreadFuncs;
		pthread_t thread;
		// lock semaphore; thread will unlock it
		sem_wait(&sem);
		pthread_create(&thread, NULL, toStart, NULL);
		// wait for the thread to unlock semaphore
		sem_wait(&sem);
		puts(arr);
		sem_post(&sem);
		sleep(1);
	}

	
	
	return 0;
}
