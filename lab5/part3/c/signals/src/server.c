#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "serverData.h"

static int socketfd;

static void CleanUp(){
	close(socketfd);
	unlink(SERVER_SOCKET_PATH);
}

static ServerData data;

void SigIntHandler(int sig){
	printf("pid = %d\n", data.pid);
}

void SigHupHandler(int sig){
	printf("uid = %d\n", data.uid);
}

void SigTermHandler(int sig){
	printf("gid = %d\n", data.gid);
}

void SigUsr1Handler(int sig){
	printf("elapsedTime: %ld seconds\n", data.elapsedTime);
}

void SigUsr2Handler(int sig){
	printf("avg load 1 minute: %f\n", data.loadavg[0]);
	printf("avg load 5 minutes: %f\n", data.loadavg[1]);
	printf("avg load 15 minutes: %f\n", data.loadavg[2]);
}

int main(){
	int ret;
	// get server start time
	time_t startTime = time(NULL);
	// get ids
	data.pid = getpid();
	data.gid = getgid();
	data.uid = getuid();

	// register signal handlers
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction)); // clear struct
	act.sa_flags = SA_RESTART;
	act.sa_handler = SigHupHandler;
	sigaction(SIGHUP, &act, NULL);
	act.sa_handler = SigIntHandler;
	sigaction(SIGINT, &act, NULL);
	act.sa_handler = SigTermHandler;
	sigaction(SIGTERM, &act, NULL);
	act.sa_handler = SigUsr1Handler;
	sigaction(SIGUSR1, &act, NULL);
	act.sa_handler = SigUsr2Handler;
	sigaction(SIGUSR2, &act, NULL);

	// create socket
	socketfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (socketfd == -1){
		perror("Error creating a socket");
		return(EXIT_FAILURE);
	}
	
	// bind socket
	struct sockaddr_un myAddr;
	memset(&myAddr, 0, sizeof(struct sockaddr_un)); // clear structure
	myAddr.sun_family = AF_UNIX;
	unlink(SERVER_SOCKET_PATH);
	strncpy(myAddr.sun_path, SERVER_SOCKET_PATH, sizeof(myAddr.sun_path) - 1); // set address
	ret = bind(socketfd, (const struct sockaddr *) &myAddr, sizeof(struct sockaddr_un));
	if (ret == -1){
		perror("failed to bind socket");
		return EXIT_FAILURE;
	}
	// satrt listerning to requests
	ret = listen(socketfd, 8);
	if (ret == -1){
		perror("Fialed to call listen");
		return EXIT_FAILURE;
	}

	// accept requests
	struct sockaddr_un clientAddr;
	socklen_t clientAddrlen = sizeof(clientAddr);
	int clientSfd;
	char str[RESPONSE_BUFFER_SIZE];
	while(1){
		clientSfd = accept(socketfd, (struct sockaddr*) (&clientAddr), &clientAddrlen);
		if (clientSfd == -1){
			perror("faield to accept request");
			return EXIT_FAILURE;

		}
		// get data
		data.elapsedTime = time(NULL) - startTime;
		// generate a response string
		getloadavg(data.loadavg, 3);
		int i = sprintf(str, "elapsedTime: %ld seconds\n", data.elapsedTime);
		i += sprintf(str+i, "avg load 1 minute: %f\n", data.loadavg[0]);
		i += sprintf(str+i, "avg load 5 minutes: %f\n", data.loadavg[1]);
		i += sprintf(str+i, "avg load 15 minutes: %f\n", data.loadavg[2]);
		// write to client
		write(clientSfd, str, i);

		close(clientSfd);
	}

	CleanUp();

	return EXIT_SUCCESS;
}
