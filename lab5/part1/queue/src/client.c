#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "serverData.h"

int main(){
	
	// get queue
	int msgid = msgget(KEY, 0666);
	// buffer to store message
	ServerData data;

	// skip old entries to get the most relevant data
	struct msqid_ds queueData;
	do {
		// revcieve message
		msgrcv(msgid, &data, sizeof(ServerData), MESSAGE_TYPE, 0);
		// get queue data
		msgctl(msgid, IPC_STAT, &queueData);
	} while (queueData.msg_qnum);

	// print serverData
	printf("elapsedTime: %ld seconds\n", data.elapsedTime);
	printf("avg load 1 minute: %f\n", data.loadavg[0]);
	printf("avg load 5 minutes: %f\n", data.loadavg[1]);
	printf("avg load 15 minutes: %f\n", data.loadavg[2]);

	return 0;
}


