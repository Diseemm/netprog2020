#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/select.h>
#define PORT 8784

int main() {
    char buffer[128];
    int sockfd, clen, clientfd, new_clientfd;
    struct sockaddr_in saddr, caddr;
    int clientfds[100];
    memset(clientfds, 0, sizeof(clientfds));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Error creating socket.\n");
        exit(0);
    } else printf("Successfully created!\n");
    
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(PORT);

    setsockopt(clientfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
    int fl = fcntl(clientfd, F_GETFL, 0);
    fl |= O_NONBLOCK;

    if (bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) != 0) {
        printf("Error binding.\n");
        exit(0);
    } else printf("Successfully bind!\n");

    int lis = listen(sockfd, 5);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
    if (lis < 0) {
        printf("Error listening.\n");
        exit(0);
    } else printf("Listening\n");

    clen = sizeof(caddr);

    while (1) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sockfd, &set);
        int maxfd = sockfd;
        for (int i = 0; i < 100; i++) {
            if (clientfds[i] > 0) FD_SET(clientfds[i], &set);
            if (clientfds[i] > maxfd) maxfd = clientfds[i];
        }
        select(maxfd+1, &set, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &set)) {
            clientfd = accept(sockfd, (struct sockaddr *) &caddr, (unsigned int *)&clen);

            setsockopt(clientfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
            int fl = fcntl(clientfd, F_GETFL, 0);
            fl |= O_NONBLOCK;
            fcntl(clientfd, F_SETFL, fl);

            for (int i = 0; i < 100; i++) {
                if (clientfds[i] == 0) {
                    clientfds[i] = clientfd;
                    break;  
                }
            }
        }

        for (int i = 0; i < 100; i++) {
            if (clientfds[i] > 0 && FD_ISSET(clientfds[i], &set)) {
                int n = recv(clientfds[i], buffer, sizeof(buffer), 0);
                if (n > 0)   printf("Client %d says: %s\n", clientfds[i], buffer);
            } else {
                printf("Client %d has disconnect.\n", clientfds[i]);
                clientfds[i] = 0;
            }
        }
    }
    close(sockfd);
    return 0;
}