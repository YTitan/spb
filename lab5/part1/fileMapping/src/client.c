#include <stdio.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

#include "serverData.h"

int main(){
	
	// open file
	int fd = open(FILEPATH, 0, 0666);
	// map file
	ServerData* data = mmap(NULL, sizeof(ServerData), PROT_READ, MAP_SHARED, fd, 0);

	// print serverData
	printf("elapsedTime: %ld seconds\n", data->elapsedTime);
	printf("avg load 1 minute: %f\n", data->loadavg[0]);
	printf("avg load 5 minutes: %f\n", data->loadavg[1]);
	printf("avg load 15 minutes: %f\n", data->loadavg[2]);

	// cloase file
	close(fd);
	return 0;
}


