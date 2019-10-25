#include <stdio.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define ARR_LEN 26
static char arr[] = "abcdefghijklmnopqrstuvwxyz";
static pthread_rwlock_t lock;

typedef void* (*ThreadFunc)(void*);

static volatile char stop = 0;
void SigIntHandler(int sig){
	stop = 1;
}

void* InvertCase(void* arg){
	// interval in microseconds
	int interval = *((int*)arg);
	while (!stop){
		// lock mutex
		pthread_rwlock_wrlock(&lock);
		// invert case
		int (*func)(int);
		if (arr[0] >= 'a' && arr[0] <= 'z')
			func = toupper;
		else 
			func = tolower;
		for (size_t i = 0; i < ARR_LEN; i++){
			arr[i] = (char) func(arr[i]);
		}
		// unlock mutex
		pthread_rwlock_unlock(&lock);
		usleep(interval);
	}
	return NULL;
}

void* PrintCapsNum(void* arg){
	int interval = *((int*) arg);
	while (!stop){
		pthread_rwlock_rdlock(&lock);
		size_t count = 0;
		for (size_t i = 0; i < ARR_LEN; ++i){
			if (arr[i] >= 'A' && arr[i] <= 'Z')
				++count;
		}
		printf("Count of capital letters in the array is %ld\n", count);
		pthread_rwlock_unlock(&lock);
		usleep(interval);
	}
}

void* InvertArray(void* arg){
	int interval = *((int*)arg);
	while (!stop){
		pthread_rwlock_wrlock(&lock);
		for (size_t i = 0; i < ARR_LEN/2; ++i){
			char temp = arr[i];
			arr[i] = arr[ARR_LEN-i-1];
			arr[ARR_LEN-i-1] = temp;
		}
		pthread_rwlock_unlock(&lock);
		usleep(interval);
	}
	return NULL;
}

static void GetMicroseconds(int argc, char* argv[], int index, int* outMilliseconds){
	int x;
	if (argc >= index + 1){
		x = atoi(argv[index]);
		if (x > 0)
			outMilliseconds[0] = x;
		else
			printf("Warning: argument %d cannot be read as an int or is equal to 0", index - 1);
	}
}

int main(int argc, char* argv[]){
	int mainMicroseconds = 1000000;
	int threadMicroseconds[] = {1000000, 1000000, 1000000};

	GetMicroseconds(argc, argv, 1, &mainMicroseconds);
	GetMicroseconds(argc, argv, 2, threadMicroseconds);
	GetMicroseconds(argc, argv, 3, threadMicroseconds + 1);
	GetMicroseconds(argc, argv, 4, threadMicroseconds + 2);

	// init rwlock
	if (pthread_rwlock_init(&lock, NULL)){
		puts("Couldn't init rwlock\n");
		return -1;
	}

	// start threads
	pthread_t invertCaseThread;
	pthread_t invertArrayThread;
	pthread_t printCapsCountThread;
	pthread_create(&invertCaseThread, NULL, InvertCase, (void*) (threadMicroseconds));
	pthread_create(&invertArrayThread, NULL, InvertArray, (void*) (threadMicroseconds + 1));
	pthread_create(&printCapsCountThread, NULL, PrintCapsNum, (void*) (threadMicroseconds + 2));

	// register sigint callback
	signal(SIGINT, SigIntHandler);
	
	while(!stop){
		pthread_rwlock_rdlock(&lock);
		puts(arr);
		pthread_rwlock_unlock(&lock);
		usleep(mainMicroseconds);
	}

	// destroy rwlock
	pthread_rwlock_destroy(&lock);

	return 0;
}
