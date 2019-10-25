#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "serverData.h"

static volatile int stop = 0;

void intHandler(int sigint){
	stop = 1;
}


int main(){
	// register sigint callback
	signal(SIGINT, intHandler);

	// get server start time
	time_t startTime = time(NULL);

	// get messages queue
	int msqid = msgget(KEY, 0666 | IPC_CREAT);

	ServerData data;
	data.mtype = MESSAGE_TYPE;


	// get id's
	data.pid = getpid();
	data.uid = getuid();
	data.gid = getgid();
	
	for (;!stop;){
		data.elapsedTime = time(NULL) - startTime;
		getloadavg(data.loadavg, 3);
		msgsnd(msqid, &data, sizeof(ServerData), 0);
		sleep(1);
	}

	msgctl(msqid, IPC_RMID, NULL);

	return 0;
}
