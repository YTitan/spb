#ifndef SERVER_DATA_H
#define SERVER_DATA_H

#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define MESSAGE_TYPE 52
#define KEY 101

typedef struct ServerData {
	long mtype;
	pid_t pid;
	uid_t uid;
	gid_t gid;
	time_t elapsedTime;
	double loadavg[3];
} ServerData;
#endif
