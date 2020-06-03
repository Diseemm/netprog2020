#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define PORT 8784

int main() {
    char buffer[128];
    char hostname[30];
    printf("Enter: ");
    scanf("%s", hostname);

    struct sockaddr_in servaddr;
    struct hostent *hostip = gethostbyname(hostname);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0) {
        printf("Error creating socket!\n");
        exit(0);
    } else printf("Created!\n");

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
    int fl = fcntl(sockfd, F_GETFL, 0);
    fl |= O_NONBLOCK;

    if (hostip == NULL) {
        printf("Unknown host!\n");
        exit(0);
    } else printf("Got host!\n");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    memcpy((char *) &servaddr.sin_addr.s_addr, hostip->h_addr_list[0], hostip->h_length);
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        printf("Can't connect!\n");
        exit(0);
    } else {
        printf("Connected!\n");
        fcntl(sockfd, F_SETFL, fl);
            for ( ; ; ) {
                printf("Enter: ");
                memset(buffer, 0, sizeof(buffer));
                scanf("%s", buffer);
                if (strcmp(buffer, "/quit") == 0) {
                    send(sockfd, buffer, strlen(buffer)+1, 0);
                    printf("Exiting!\n");
                    memset(buffer, 0, sizeof(buffer));
                    shutdown(sockfd, SHUT_WR);
                    close(sockfd);
                    exit(0);
                } else {
                    send(sockfd, buffer, strlen(buffer)+1, 0);
                    memset(buffer, 0, sizeof(buffer));
                    if (recv(sockfd, buffer, sizeof(buffer), 0) == 0) {
                        printf("Error!\n");
                        exit(0);
                    } else {     
                        printf("From server: %s\n", buffer);
                    }
                }
            }
    }

    close(sockfd);
    return 0;
}