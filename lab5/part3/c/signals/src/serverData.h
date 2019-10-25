#ifndef SERVER_DATA_H
#define SERVER_DATA_H

#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define SERVER_SOCKET_PATH "/tmp/my_server_socket.socket"
#define RESPONSE_BUFFER_SIZE 512 

typedef struct ServerData {
	long mtype;
	pid_t pid;
	uid_t uid;
	gid_t gid;
	time_t elapsedTime;
	double loadavg[3];
} ServerData;
#endif
