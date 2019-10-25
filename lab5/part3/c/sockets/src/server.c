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

void intHandler(int sigint){
	CleanUp();
	exit(EXIT_SUCCESS);
}

int main(){
	int ret;
	// get ids
	pid_t pid = getpid();
	gid_t gid = getgid();
	uid_t uid = getuid();
	// register sigint callback
	signal(SIGINT, intHandler);

	// get server start time
	time_t startTime = time(NULL);

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
		// generate a response string
		double loadavg[3];
		getloadavg(loadavg, 3);
		int i = sprintf(str, "elapsedTime: %ld seconds\n", time(NULL) - startTime);
		i += sprintf(str+i, "avg load 1 minute: %f\n", loadavg[0]);
		i += sprintf(str+i, "avg load 5 minutes: %f\n", loadavg[1]);
		i += sprintf(str+i, "avg load 15 minutes: %f\n", loadavg[2]);

		write(clientSfd, str, i);

		close(clientSfd);
	}

	CleanUp();

	return EXIT_SUCCESS;
}
