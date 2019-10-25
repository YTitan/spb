#ifndef SERVER_DATA_H
#define SERVER_DATA_H

#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define FILEPATH "/home/titan/itmo/spb/lab5/part1/fileMapping/file"

typedef struct ServerData {
	pid_t pid;
	uid_t uid;
	gid_t gid;
	time_t elapsedTime;
	double loadavg[3];
} ServerData;
#endif
