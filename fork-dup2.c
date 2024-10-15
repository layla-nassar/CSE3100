#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <ctype.h> 
#include <sys/stat.h>
#define MAX_LINE 1024

int main(int argc, char *argv[])
{

	if(argc!=2)
	{
		printf("Usage: %s message\n", argv[0]);
		return -1; 
	}
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {

		//Make sure when the file is created, the user has the read and write access 
		int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		if(fd < 0)
		{ 
			printf("Cannot open the file\n");
			return -1;
		} 
        char buffer[MAX_LINE];
        strcpy(buffer, argv[1]);

		dup2(fd, STDOUT_FILENO);

		for(int i = 0; buffer[i]; i++) {
			buffer[i] = toupper(buffer[i]);
		}

		printf("%s\n", buffer);

		close(fd);

		exit(0);
	}

	printf("%s\n", argv[1]);
	return 0;
}
