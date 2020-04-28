#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main() {
    char hostname[128];
    printf("Enter: ");
    scanf("%s", hostname);

    struct hostent *host_info = gethostbyname(hostname);
    struct in_addr *address = (struct in_addr*) (host_info->h_addr_list[0]);
    
    printf("The ip for page %s is: %s\n", hostname, inet_ntoa(*address));
    return 0;
}