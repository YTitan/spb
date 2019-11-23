#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	
	int portnum;
	if (argc < 4){
		printf("usage %s hostname port [dirname...]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0){
		perror("Failed to open socket");
		exit(EXIT_FAILURE);
	}

	struct hostent* server = gethostbyname(argv[1]);

	if (!server){
		perror("No such host");
		exit(EXIT_FAILURE);
	}

	portnum = atoi(argv[2]);

	struct sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof (servAddr));
	servAddr.sin_family = AF_INET;
	bcopy((char*)server->h_addr, (char*)&servAddr.sin_addr.s_addr, server->h_length);
	servAddr.sin_port = htons(portnum);

	if (connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("Failed to connect to server");
		exit(EXIT_FAILURE);
	}

	for (int i = 3; argv[i]; i++){
		write(sock, argv[i], strlen(argv[i]));
		write(sock, "\n", 1);
	}
	//fsync(sock);
	shutdown(sock, SHUT_WR);	

	char buffer[256];
	while (read(sock, buffer, 255)){
		puts(buffer);
	}
	close(sock);



	return EXIT_SUCCESS;
}
