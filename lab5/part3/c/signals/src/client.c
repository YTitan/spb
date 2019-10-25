#include <stdio.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <stdlib.h>

#include "serverData.h"

int main(){
	
	char buffer[RESPONSE_BUFFER_SIZE];
	struct sockaddr_un serverAddr;
	int sfd;
	int ret;

	// create local socket
	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sfd == -1){
		perror("Failed to create socket");
		exit(EXIT_FAILURE);
	}

	// set addr to connect to
	memset(&serverAddr, 0, sizeof(struct sockaddr_un));
	serverAddr.sun_family = AF_UNIX;
	strncpy(serverAddr.sun_path, SERVER_SOCKET_PATH, sizeof(serverAddr.sun_path) - 1);
	
	// connect
	ret = connect(sfd, (const struct sockaddr*) &serverAddr, sizeof(struct sockaddr_un));
	if (ret == -1){
		perror("Failed to connect to server");
		exit(EXIT_FAILURE);
	}

	read(sfd, buffer, RESPONSE_BUFFER_SIZE);

	puts(buffer);	

	close(sfd);
	return EXIT_SUCCESS;
}


