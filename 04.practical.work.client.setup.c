#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define PORT 8784

int main() {
    struct sockaddr_in servaddr;
    struct hostent *hostip;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0) {
        printf("Error creating socket!\n");
        exit(0);
    } else printf("Created!\n");

    if ((hostip = gethostbyname("www.usth.edu.vn")) == NULL){
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
    } else printf("Connected!\n");

    return 0;
}