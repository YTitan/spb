#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>

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

	// open file
	int fd = open (FILEPATH, O_RDWR | O_CREAT, 0666);	
	// write smth to file 'cause mmap'ing an emty file results in an error
	write(fd, "hello world\n", 12);

	// map file
	ServerData* data = mmap(NULL, sizeof(ServerData), PROT_WRITE, MAP_SHARED, fd, 0);
	// get id's
	data->pid = getpid();
	data->uid = getuid();
	data->gid = getgid();
	
	for (;!stop;){
		data->elapsedTime = time(NULL) - startTime;
		getloadavg(data->loadavg, 3);
		sleep(1);
	}

	// unmap file
	munmap(data, sizeof(ServerData));
	// close file
	close(fd);
	return 0;
}
