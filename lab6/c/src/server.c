#include <sys/types.h>
#include <pthread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>

#include "clientServer.h"

int proceed = 1;

void SigIntHandler(int signal){
	proceed = 0;
}
void ProcessClientRequest(int clientSocket){
	FILE* cf = fdopen(clientSocket, "a+");
	char* line = (char*) malloc(4096);
	size_t lineLen = 4096;
	ssize_t n;
	while ((n = getline(&line, &lineLen, cf)) > 0){
		if (line[n-1] == '\n')
			line[n-1] = 0;
		// generate response
		DIR* dir = opendir(line);
		if (dir){
			struct dirent* direntry;
			while (direntry = readdir(dir)){
				write(clientSocket, direntry->d_name, sizeof(direntry->d_name) - 1);
			}
			closedir(dir);
		}

	}
	// close socket
	close(clientSocket);
}

void* ThreadProcessRequest(void* args){
	int* sock = (int*) args;
	ProcessClientRequest(*sock);

	free(sock);
}

int main(){

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = SigIntHandler;
	sigaction(SIGINT, &act, NULL);
	
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock <= 0){
		perror("Failed to open socket");
		exit(EXIT_FAILURE);
	}

	// initialize socket address
	struct sockaddr_in addr;
	memset(&addr, 0 , sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(SERVER_PORT);

	// bind socket
	if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0){
		perror("Failed to bind socket");
		exit(EXIT_FAILURE);
	}

	// start listening
	listen(sock, 10);

	// accept connections and dispatch them to threads
	struct sockaddr_in clientAddr;
	int clientAddrLen;
	while (proceed){
		int* client = (int*) malloc(sizeof(int));
		*client = accept(sock, (struct sockaddr*) &clientAddr, &clientAddrLen);
		if (*client > 0){
			// create thread to process the request
			pthread_t thread;
			pthread_create(&thread, NULL, ThreadProcessRequest, client);
		}
		else {
			perror("Failed to accept connection");
		}
	}

	close(sock);

	return EXIT_SUCCESS;
}
