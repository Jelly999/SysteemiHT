/**/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * Otsikkotiedot: wish
 * Yksinkertaisen wish shellin luominen
 * Tekijät: Nico Aalto & Shan Shan Yang
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>



size_t getCommand(size_t, size_t, char*);
size_t getCommandline(char *, int, size_t, size_t, char*);
int commandCheck(char *, char(*)[64]);
int runCommand(char **, char(*)[64], int);
int myCD(char **, char *);
int myPath(char **, char(*)[64]);
int argumentsCheck(char **);

int main(int argc, char **argv) {
  //
	char error_message[30] = "An error has occured\n";

	// i used for index and j for executing from file
	int i = 0,j = 1;

	// Allocating buffer
	size_t command;
	size_t buffersize = 4096;
	char *buffer;
	if (!(buffer = (char*)malloc(buffersize * sizeof(char)))) {
		write(STDERR_FILENO, "Unable to malloc buffer\n", strlen("Unable to malloc buffer\n"));
		exit(-1);
	}
	// strtok
	char *strtok_pointer;
	char *arguments[64];

  // Commandpath
	char commandpath[32][64]={"/bin/"};
	for (int i = 1; i < 32; i++) {
			memset(commandpath[i],0,64);
	}
  // Working directory
	char path[256];
	if (getcwd(path, sizeof(path)) == NULL) {
		write(STDERR_FILENO, "Could not get working directory\n", strlen("Could not get working directory\n"));
		exit(-1);
	}

	system("clear"); // does not close program

	printf("Shell!\n");

	while(1) {
			printf("Working directory: %s\n", path);
			i = 0;
			while(commandpath[i][0] != '\0') {
				printf("Commandpath %d: %s\n", i+1, commandpath[i]);
				i++;
			}

      //Checking amount of arguments
			if (argc == 1) {
				// Executing from user input
				command = getCommand(command, buffersize, buffer);
			} else {
        // Executing from file
				command = getCommandline(argv[1], j, command, buffersize, buffer);
				if (command == -1) {
					exit(0);
				}
				j++;
			}

			strtok(buffer, "\n");
			strtok_pointer = strtok(buffer, " ");
			arguments[0] = strtok_pointer;
			i = 1;
			printf("Command given: ");
			printf("%s ", arguments[0]);
			while ((strtok_pointer = strtok(NULL, " ")) != NULL) {
				printf("%s ", strtok_pointer);
				arguments[i] = strtok_pointer;
				i++;
			}
			arguments[i] = NULL;
			printf("\n");
			

			// Using switch case to use own funtions and for exit
			switch(commandCheck(arguments[0],commandpath)) {
				case 0: // Exit program
					printf("Good bye\n");
					free(buffer);
					exit(0);
					break;
				case 1: // Execute normally
					i=0;
					i = argumentsCheck(arguments);

					printf("Executing command: %s\n", arguments[0]);
					printf("\n");

					if (i != 0) {
						runCommand(arguments, commandpath, i);
					} else {
						runCommand(arguments, commandpath, i);
					}
					break;
				case 2: // myCD function 
					if (myCD(arguments, path) != 0) {
						write(STDERR_FILENO, error_message, strlen(error_message));
					}
					break;
				case 3: // myPath function
					myPath(arguments, commandpath);
					break;
				case -1: // Error handling
					write(STDERR_FILENO, "Command not found\n", strlen("Command not found\n"));
					break;
			}
			printf("\n\n");
	}

	return(-1);
}

// Getting all arguments when Executing from file
size_t getCommandline(char *argv, int j, size_t command, size_t buffersize, char* buffer) {
	FILE *fp;
	fp = fopen(argv, "r");
	for (int i = 0; i<j; i++) {
		command = getline(&buffer, &buffersize, fp);
	}
	fclose(fp);
	return(command);
}


//get input from user
size_t getCommand(size_t command, size_t buffersize, char* buffer) {
	printf("wish> ");
	command = getline(&buffer, &buffersize, stdin);
	return(command);
}

// Changes coomandpath
int myPath(char** arguments, char (*commandpath)[64]) {

	for (int i = 0; i < 32; i++) {
		commandpath[i][0] = '\0';
	}

	int i = 0;
	while(1) {
		if(arguments[i] == NULL){
			break;
		}
		strcpy(commandpath[i-1], arguments[i]);
		i++;
	}
	return(0);
}


// Changing current directory, returns 0 if successful
int myCD(char** arguments, char* path) {
	char* a = NULL;
	char newpath[64] = {};

	if (arguments[1] == NULL) {
		write(STDERR_FILENO, "No arguments\n", strlen("No arguments\n"));
		return(-1);
	} else if (arguments[2] != NULL) {
		write(STDERR_FILENO, "Too many arguments\n", strlen("Too many arguments\n"));;
		return(-1);
	} else {
		if (strcmp(arguments[1], "..") == 0) {
			
			a = strrchr(path, '/');
			if (a) {
				(*a) = '\0';
				chdir(path);
				return(0);
			} else {
				return(-1);
			}
		} else if (arguments[1][0] == '/') {
			if(!chdir(arguments[1])) {
				strcpy(path, arguments[1]);
				return(0);
			} else {
				return(-1);
			}
		} else {

			strcat(newpath, path);
			strcat(newpath, "/");
			strcat(newpath, arguments[1]);
			if (!chdir(newpath)) {
				strcpy(path, newpath);
				return(0);
			} else {
				return(-1);
			}
		}
	}

}

// Checks if given command is exit, cd or path
int commandCheck(char *command, char (*commandpath)[64]) {

	if (strcmp(command, "exit") == 0 ) {
		return(0);
	} else if (strcmp(command, "cd") == 0 ) {
		return(2);
	} else if (strcmp(command, "path") == 0 ) {
		return(3);
	}

	char file_location[4096];
	int i = 0;

	while(commandpath[i][0] != '\0') {
		strcpy(file_location, commandpath[i]);
		strcat(file_location, command);
		if (!access(file_location, X_OK)) {
			return(1);
		}
		i++;
	}
	return(-1);
}


int argumentsCheck(char **arguments) {
	int i = 0;
	while(arguments[i]) {
		if (strcmp(arguments[i], ">") == 0) {
			arguments[i] = NULL;
			return(i);
		}
		i++;
	}
	return(0);
}

// Runs command in child process
int runCommand(char **arguments, char (*commandpath)[64], int j) {
	char file_location[4096];
	int i = 0;
	while(arguments[i]) {
		strcpy(file_location, commandpath[i]);
		strcat(file_location, arguments[0]);
		if (!access(file_location, X_OK)) {
			break;
		}
		i++;
	}

	char command[4096];
	strcpy(command, commandpath[i]);
	strcat(command, arguments[0]);
	pid_t pid=fork();


	if (pid==0) {
		int fd;
		if (j != 0) {
			fd = open(arguments[j+1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			dup2(fd, 1);			
		}
		execv(command, arguments);
		close(fd);
		exit(127);
	} else {
		waitpid(pid, 0, 0);
	}

	return(0);
}

// Lähteenä käytetty gvithubista käyttäjän petterigit lähdekoodia