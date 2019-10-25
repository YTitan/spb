#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "serverData.h"

int main(){
	
	// get key
	key_t key = ftok(FTOK_FILEPATH, FTOK_INT);
	// get shared memory segment
	int shmid = shmget(key, sizeof(ServerData), 0666);
	if (shmid < 0){
		puts("Server is not running\n");
		return -1;
	}
	// get poin
	ServerData* data = shmat(shmid, (void*) 0, 0);

	// print serverData
	printf("elapsedTime: %ld seconds\n", data->elapsedTime);
	printf("avg load 1 minute: %f\n", data->loadavg[0]);
	printf("avg load 5 minutes: %f\n", data->loadavg[1]);
	printf("avg load 15 minutes: %f\n", data->loadavg[2]);
	return 0;
}


