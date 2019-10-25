#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

typedef struct File {
	char* filename;
	FILE* file;		
} File;

void sigintIgnore(int sig){
	printf("Ignoring an interrupt: %d", sig);
}

int main(int argv, char* argc[]){
	// flags
	bool append = false;
	bool ignoreInterrupts = false;
	bool ignoreFlags = false;
	
	// keep track of opened files
	File* files = (File*) calloc((argv - 1), sizeof(File));

	int returnCode = 0;

	// parse flags
	for (int i = 1; i < argv; ++i){
		char* str = argc[i];
		size_t len = strlen(str);
		if (str[0] == '-' && !ignoreFlags && len > 1){
			for (size_t j = 1; j < len; ++j){
				switch (str[j]){
					case 'a':
						append = true;
						break;
					case 'i':
						ignoreInterrupts = true;
						break;
					case '-':
						if ( j == 1)
							if ( len == 2)
								ignoreFlags = true;
							else {
							}
						else {
							fprintf (stderr, "Unrecognized option: %s\n", str);
							return 1;
						}
						break;
					default:
						fprintf( stderr, "Invalid option: %c\n", str[j]);
						return 1;
					}
				}	
		}
		else {
			File* file = files + i -1;
			file->filename = str;
		}

	}
	// set ignoring function for interrupts
	if (ignoreInterrupts)
		signal(SIGINT, sigintIgnore);
	// set open mode
	char mode[2] = {append?'a':'w', 0};

	// open files
	for (int i = 0; i < argv -1; ++i){
		File* file = files + i;
		if (file->filename){
			file->file = fopen(file->filename, mode);
			if (!file->file)
				++returnCode;
		}
	}

	// write to files
	// while reading from stin in chunks
	char buffer[256];
	while (fgets(buffer, 256, stdin)){
		// write to files
		for (int i = 0; i < argv - 1; ++i){
			File* f = files + i;
			if (f->file)
			fputs(buffer, f->file);		
		}
		// write to stdout
		fputs(buffer, stdout);
	}


	// free memory and close files
	for (int i = 0; i < argv - 1; ++i){
		File* file = files + i;
		if (file->file)
			fclose(file->file);
	}
	free (files);

	return returnCode;
}
