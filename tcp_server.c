#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>


#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(argv[1]));
    
    // Bind the socket to the address and port number
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", atoi(argv[1]));
    
    while (1) {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Read data from client
        valread = read(new_socket, buffer, BUFFER_SIZE);
        printf("Received message from client: %s\n", buffer);
        
        // Convert characters to uppercase
        for (int i = 0; i < valread; i++) {
            buffer[i] = toupper(buffer[i]);
        }

        // Send modified data back to client
        send(new_socket, buffer, strlen(buffer), 0);

        // Check if client wants to terminate
        if (strncmp(buffer, "QUIT", 4) == 0) {
            printf("Server is closing the connection\n");
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
    }
    
    // Close the socket
    close(server_fd);
    
    return 0;
}