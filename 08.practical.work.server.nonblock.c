#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#define PORT 8784

int main() {
    char buffer[128];
    int sockfd, clen, clientfd;
    struct sockaddr_in saddr, caddr;

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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    if (listen(sockfd, 5) < 0) {
        printf("Error listening.\n");
        exit(0);
    } else printf("Listening\n");

    clen = sizeof(caddr);
    while (1) {
        clientfd = accept(sockfd, (struct sockaddr *) &caddr, (unsigned int *)&clen);
        if (clientfd > 0) {
            printf("Accepted!\n");
            fcntl(clientfd, F_SETFL, fl);
            for ( ; ; ) {
                memset(buffer, 0, sizeof(buffer));
                int n = recv(clientfd, buffer, sizeof(buffer), 0);
                if (strcmp(buffer, "/quit") == 0) {
                    memset(buffer, 0, sizeof(buffer));
                    printf("Client exited!\n");
                    shutdown(clientfd, SHUT_WR);
                    close(clientfd);
                } else {
                    int firstChar = 0;
                    for (int i = 0; i < n; i++) {
                        if (buffer[i] == '\0') {
                            printf("From client: %s\n", &buffer[firstChar]);
                            firstChar = i+1;
                        }
                    }
                    memset(buffer, 0, sizeof(buffer));
                    printf("Enter: ");
                    scanf("%s", buffer);
                    if (strcmp(buffer, "/dc") == 0) {
                        exit(0);
                    }
                    send(clientfd, buffer, strlen(buffer)+1, 0);
                }
            }
        } else printf("Not Accepted!!!!!\n");   
    }
    close(sockfd);
    return 0;
}