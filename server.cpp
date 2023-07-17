#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Set up server address
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    portno = 12345; // Change this port number if needed
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error on binding");
        exit(1);
    }

    // Listen for client connections
    listen(sockfd, 5);

    clilen = sizeof(cli_addr);
    printf("Server: Waiting for client connection...\n");

    // Accept client connection
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("Error on accept");
        exit(1);
    }

    printf("Server: Client connected\n");

    // Generate random number between 50,000 and 80,000
    int random_num = rand() % 30001 + 50000;

    // Send random number to client
    n = write(newsockfd, &random_num, sizeof(random_num));
    if (n < 0) {
        perror("Error writing to socket");
        exit(1);
    }

    printf("Server: Random number sent to client: %d\n", random_num);

    close(newsockfd);
    close(sockfd);

    return 0;
}
