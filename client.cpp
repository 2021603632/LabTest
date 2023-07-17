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
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char buffer[256];

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
    serv_addr.sin_port = htons(portno);

    // Convert IP address to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &(serv_addr.sin_addr)) <= 0) {
        perror("Invalid address");
        exit(1);
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    // Receive random number from server
    int random_num;
    n = read(sockfd, &random_num, sizeof(random_num));
    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    }

    printf("Client: Received random number from server: %d\n", random_num);

    close(sockfd);

    return 0;
}
