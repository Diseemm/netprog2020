#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define PORT 8784

extern ssize_t send(), recv();

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

    if (bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) != 0) {
        printf("Error binding.\n");
        exit(0);
    } else printf("Successfully bind!\n");
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    if (listen(sockfd, 5) < 0) {
        printf("Error listening.\n");
        exit(0);
    } else printf("Listening\n");

    clen = sizeof(caddr);
    clientfd = accept(sockfd, (struct sockaddr *) &caddr, &clen);
    
    if (clientfd < 0) {
        printf("Error accepting connection.\n");
        exit(0);
    } else {    
        printf("Accepted!\n");
        while (1) {
            int n = recv(clientfd, buffer, sizeof(buffer), 0);
            if (n == 0) {
                printf("Error!!!\n");
                exit(0);
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
                if (strcmp(buffer, "/dc") == 0)    break;
                send(clientfd, buffer, strlen(buffer)+1, 0);
            }
        }
    }

    close(clientfd);
    return 0;
}