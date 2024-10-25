
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h>

// redirect standard input to the specified file
void redirectStdin(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening the file\n");
        exit(-1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

// redirect standard output to the specified file
void redirectStdout(const char *filename)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0)
    {
        perror("Error opening the file\n");
        exit(-1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

// if word is an arithmetic sequence word, return 1 else return 0
int arithmetic(char *word)
{
    int len = strlen(word);
    if (len < 3) {  
        return 0;
    }

    int diff = word[1] - word[0];
    for (int i = 2; i < len; i++) {
        if (word[i] - word[i - 1] != diff) {
            return 0;
        }
    }
    return 1;
}


