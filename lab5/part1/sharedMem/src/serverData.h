#ifndef SERVER_DATA_H
#define SERVER_DATA_H

#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define FTOK_FILEPATH "/home/titan/itmo"
#define FTOK_INT 23

typedef struct ServerData {
	pid_t pid;
	uid_t uid;
	gid_t gid;
	time_t elapsedTime;
	double loadavg[3];
} ServerData;
#endif
