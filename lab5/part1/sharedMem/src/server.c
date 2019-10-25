#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "serverData.h"

static volatile int stop = 0;

void SigIntHandler(int sig){
	stop = 1;
}

int main(){

	// get server start time
	time_t startTime = time(NULL);

	// register sigint callback
	signal(SIGINT, SigIntHandler);

	// get key
	key_t key = ftok(FTOK_FILEPATH, FTOK_INT);
	// get sharedMemory segment
	int shmid = shmget(key, sizeof(ServerData), 0666 | IPC_CREAT);

	ServerData* data = (ServerData*) shmat(shmid, (void*)0, 0);

	// get id's
	data->pid = getpid();
	data->uid = getuid();
	data->gid = getgid();
	
	for (;!stop;){
		data->elapsedTime = time(NULL) - startTime;
		getloadavg(data->loadavg, 3);
		sleep(1);
	}

	// detach from shared memory segmment
	shmdt(data);
	// destroy memory segment
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}
