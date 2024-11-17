/* server.c -- a stream socket server demo */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "socketio.c"

#define PORT "3119" // the port users will be connecting to
#define BACKLOG 10  // how many pending connections queue will hold
#define MSG_BUF_SIZE 100
#define MAX_VALUE 1000

typedef struct {
    int v;
    int attempts;
    char message[MSG_BUF_SIZE];
} gmn_t;

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

void gmn_init(gmn_t *pg)
{
    pg->v = 34;  // Hardcode the hidden number to 34
    pg->attempts = 0;
    strcpy(pg->message, "You have not got the right number yet.\n");
}

int gmn_check(gmn_t *pg, int guess)
{
    pg->attempts++;
    if (pg->v == guess) {
        snprintf(pg->message, MSG_BUF_SIZE,
                "It took you %d attempt(s) to guess the number %d.\n",
                pg->attempts, pg->v);
        return 0;
    }
    return (pg->v > guess) ? 1 : -1;
}

void *thread_main(void *arg)
{
    int sockfd = *((int *)arg);
    free(arg);

    gmn_t gmn;
    gmn_init(&gmn);

    char buf[MSG_BUF_SIZE];
    int guess, result;

    // Send maximum value to client
    snprintf(buf, sizeof(buf), "%d\n", MAX_VALUE);
    if (send_all(sockfd, buf, strlen(buf)) < 0) {
        perror("Failed to send max value");
        close(sockfd);
        return NULL;
    }

    // Game loop
    while (1) {
        // Receive guess
        if (recv_lines(sockfd, buf, sizeof(buf)) < 0) {
            perror("Failed to receive guess");
            break;
        }
        if (get_number(buf, &guess) < 0) {
            perror("Failed to parse guess");
            break;
        }
        printf("DEBUG: Received guess: %d\n", guess);

        // Check guess
        result = gmn_check(&gmn, guess);
        printf("DEBUG: Hidden number: %d, Result to send: %d, Attempts: %d\n", gmn.v, result, gmn.attempts);

        // Send result
        snprintf(buf, sizeof(buf), "%d\n", result);
        if (send_all(sockfd, buf, strlen(buf)) < 0) {
            perror("Failed to send result");
            break;
        }

        // Send final message if correct
        if (result == 0) {
            if (send_str(sockfd, gmn.message) < 0) {
                perror("Failed to send final message");
            }
            break;
        }
    }

    close(sockfd);
    return NULL;
}

int main(void)
{
    int sockfd, *new_fd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    char s[INET6_ADDRSTRLEN];
    int yes = 1;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while (1) {
        sin_size = sizeof their_addr;
        new_fd = malloc(sizeof(int));
        *new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (*new_fd == -1) {
            perror("accept");
            free(new_fd);
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);

        pthread_t tid;
        if (pthread_create(&tid, NULL, thread_main, new_fd) != 0) {
            perror("pthread_create");
            free(new_fd);
        }
    }

    return 0;
}
