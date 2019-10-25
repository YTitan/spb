#include <stdio.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/sem.h>
#include <signal.h>

#include <errno.h>
#include <string.h>

#define ARR_LEN 26
static char arr[] = "abcdefghijklmnopqrstuvwxyz";
#define SEM_KEY 101
static int semid;

typedef void* (*ThreadFunc)(void*);

struct sembuf GetLockOp(){
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = 1;
	op.sem_flg = 0;
	return op;
}

struct sembuf GetUnlockOp(){
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = -1;
	op.sem_flg = 0;
	return op;
}

struct sembuf GetWaitForUnlockOp(){
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = 0;
	op.sem_flg = 0;
}

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
	struct sembuf unlockOp = GetUnlockOp();
	semop(semid, &unlockOp, 1);
	return NULL;
}

void* InvertArray(void* arg){
	for (size_t i = 0; i < ARR_LEN/2; ++i){
		char temp = arr[i];
		arr[i] = arr[ARR_LEN-i-1];
		arr[ARR_LEN-i-1] = temp;
	}
	// unlock semaphore
	struct sembuf unlockOp = GetUnlockOp();
	semop(semid, &unlockOp, 1);
	return NULL;
}

static volatile char stop = 0;
void SigIntHandler(int sig){
	stop = 1;
}

void PrintSemval(){
	printf("semval = %d\n", semctl(semid, 0, GETVAL));
}

int main(){
	// init semaphore
	semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
	// lock semaphore operation
	struct sembuf lockOp = GetLockOp();
	// unlock semaphore operation
	struct sembuf unlockOp = GetUnlockOp();
	// wait-for-unlock(0) semaphore operation
	struct sembuf waitForUnlockOp = GetWaitForUnlockOp();

	ThreadFunc threadFuncPtrs[] = {InvertCase, InvertArray};
	size_t numThreadFuncs = 2;
	size_t threadFuncIter = 0;
	while(!stop){
		ThreadFunc toStart = threadFuncPtrs[threadFuncIter];
		threadFuncIter = (threadFuncIter + 1) % numThreadFuncs;
		pthread_t thread;
		// lock semaphore; thread will unlock it
		semop(semid, &lockOp, 1);
		pthread_create(&thread, NULL, toStart, NULL);
		// wait for the thread to unlock semaphore
		semop(semid, &waitForUnlockOp, 1);
		semop(semid, &lockOp, 1);
		puts(arr);
		semop(semid, &unlockOp, 1);
		sleep(1);
	}

	// destroy System V semaphore
	semctl(semid, 1, IPC_RMID);	
	return 0;
}
