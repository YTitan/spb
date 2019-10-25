#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

#define WC_PATH "/usr/bin/wc"

int main(int argc, char* argv[]){
	int ret;

	if (argc == 1){
		puts("usage:\n ./app filename");
		exit(EXIT_FAILURE);
	}

	// creaete a pipe
	int pipefd[2];
	ret = pipe(pipefd);
	if (ret == -1){
		perror("Failed to create a pipe");
		exit(EXIT_FAILURE);
	}
	
	// fork process
	pid_t cpid = fork();
	if (cpid == -1){
		perror("Failed to create child process");
		exit(EXIT_FAILURE);
	}

	if (cpid){ // this is parent process
		// redirect stdout to child process
		ret = dup2(pipefd[1], STDOUT_FILENO);
		if (ret == -1){
			perror("Couldn't redirect parent process' stdout to child process");
			exit(EXIT_FAILURE);
		}
		// close pipe write descriptor - because it's open as stdout
		close(pipefd[1]);
		// close pipe read descriptor - parent process doesn't need it
		close(pipefd[0]);

		/* read from file and write to child process*/
		FILE* file = fopen(argv[1], "r");
		if (!file){
			perror("couldn't open file");
			exit(EXIT_FAILURE);
		}
		char c;
		do {
			fgetc(file);
			c = (char) fgetc(file);
			if (c != EOF)
				putchar(c);
		} while (c != EOF);
		fclose(file);
		// close output stream
		fclose(stdout);
		// wait for the child process to finish so that
		// user can continue using bash only after child process
		// finishes outputing its output
		int wstatus;
		waitpid(cpid, &wstatus, 0);
	}
	else { // this is child process
		// close pipe's write fd - child doesn't need it
		close(pipefd[1]);
		// redirect stdin
		ret = dup2(pipefd[0], STDIN_FILENO);
		if (ret == -1){
			perror("Couldn't redirect stdin");
			exit(EXIT_FAILURE);
		}
		
		// swap process image for wc
		ret = execl(WC_PATH, "wc", (char*) NULL);
		if (ret == -1){
			perror("couldn't swap child process' image for wc's one");
			exit(EXIT_FAILURE);
		}
	}
	return EXIT_SUCCESS;
}
