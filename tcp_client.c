#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Set server address and port number
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Read data from keyboard
    while(1) {
        printf("Enter message to send to server (type QUIT to exit): ");
if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
    perror("fgets error");
    exit(EXIT_FAILURE);
}
        // Send data to server
        send(sock, buffer, strlen(buffer), 0);

        // Check if user wants to terminate
        if (strncmp(buffer, "QUIT", 4) == 0) {
            printf("Client is closing the connection\n");
            break;
        }

        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Receive modified data from server
        valread = read(sock, buffer, BUFFER_SIZE);
        printf("Modified message from server: %s\n", buffer);

        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);
    }

    // Close the socket
    close(sock);

    return 0;
}