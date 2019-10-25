#include <stdio.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define ARR_LEN 26
static char arr[] = "abcdefghijklmnopqrstuvwxyz";
static pthread_mutex_t mutex;

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
		pthread_mutex_lock(&mutex);
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
		pthread_mutex_unlock(&mutex);
		usleep(interval);
	}
	return NULL;
}

void* InvertArray(void* arg){
	int interval = *((int*)arg);
	while (!stop){
		pthread_mutex_lock(&mutex);
		for (size_t i = 0; i < ARR_LEN/2; ++i){
			char temp = arr[i];
			arr[i] = arr[ARR_LEN-i-1];
			arr[ARR_LEN-i-1] = temp;
		}
		pthread_mutex_unlock(&mutex);
		usleep(interval);
	}
	return NULL;
}

static void GetMicroseconds(int argc, char* argv[], int index, int* outMicroseconds){
	int x;
	if (argc >= index + 1){
		x = atoi(argv[index]);
		if (x > 0)
			outMicroseconds[0] = x;
		else
			printf("Warning: argument %d cannot be read as an int or is equal to 0", index - 1);
	}
}

int main(int argc, char* argv[]){
	int mainMicroseconds = 1000000;
	int threadMicroseconds[2] = {1000000, 1000000};

	GetMicroseconds(argc, argv, 1, &mainMicroseconds);
	GetMicroseconds(argc, argv, 2, threadMicroseconds);
	GetMicroseconds(argc, argv, 3, threadMicroseconds + 1);

	// init mutex
	if (pthread_mutex_init(&mutex, NULL)){
		puts("Couldn't init mutex\n");
		return -1;
	}

	// start threads
	pthread_t invertCaseThread;
	pthread_t invertArrayThread;
	pthread_create(&invertCaseThread, NULL, InvertCase, (void*) (threadMicroseconds));
	pthread_create(&invertArrayThread, NULL, InvertArray, (void*) (threadMicroseconds + 1));

	// register sigint callback
	signal(SIGINT, SigIntHandler);
	
	while(!stop){
		pthread_mutex_lock(&mutex);
		puts(arr);
		pthread_mutex_unlock(&mutex);
		usleep(mainMicroseconds);
	}

	// destroy mutex
	pthread_mutex_destroy(&mutex);

	return 0;
}
