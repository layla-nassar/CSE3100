/* client.c -- a stream socket client demo */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "socketio.c"

#define PORT "3119"
#define LINE_SIZE 100

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int get_number(char *s, int *pv)
{
    if (sscanf(s, "%d\n", pv) != 1)
        return -1;
    return 0;
}

void die(const char *s)
{
    if (errno)
        perror(s);
    else 
        fprintf(stderr, "Error: %s\n", s);
    exit(EXIT_FAILURE);
}

int send_int(int sid, int v)
{
    char buf[64];
    int n = snprintf(buf, sizeof(buf), "%d\n", v);
    return send_all(sid, buf, n);
}

int main(int argc, char *argv[])
{
    int sockfd;
    char buf[LINE_SIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("client: connect");
            close(sockfd);
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo);

    int min = 1, max = 1000, guess, result = 0, attempts = 0;

    // Receive max value from the server
    if (recv_lines(sockfd, buf, sizeof(buf)) < 0) {
        die("Failed to receive maximum value");
    }
    if (get_number(buf, &max) < 0 || max <= 0) {
        die("Invalid maximum value received");
    }
    printf("%d\n", max);

    // Binary guessing loop
    while (min <= max) {
        guess = min + (max - min) / 2;  // Updated midpoint calculation
        printf("My guess: %d\n", guess);

        // Send guess to the server
        if (send_int(sockfd, guess) < 0) {
            die("Failed to send guess");
        }

        // Receive response
        if (recv_lines(sockfd, buf, sizeof(buf)) < 0) {
            die("Failed to receive result");
        }
        if (get_number(buf, &result) < 0) {
            die("Invalid result received");
        }
        printf("%d\n", result);

        // Increment attempts
        attempts++;

        // Adjust range based on result
        if (result == 0) {
            // Correct guess
            break;
        } else if (result == 1) {
            min = guess + 1;
        } else if (result == -1) {
            max = guess - 1;
        }
    }

    // Print final message
    printf("It took you %d attempt(s) to guess the number %d.\n", attempts, guess);

    close(sockfd);
    return 0;
}
