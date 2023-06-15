#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define CLOSE_MESSAGE "exit"

void *connection_handler(void *);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for incoming connections...\n");

    while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))) {
        printf("New client connected...\n");
        int valread;
        char buffer[1024] = {0};
        char *welcome_message = "Welcome to the chat room!\n";

        send(new_socket, welcome_message, strlen(welcome_message), 0);

        while ((valread = read(new_socket, buffer, 1024)) > 0) {
           printf("Received message: %s", buffer);
           if (strncmp(CLOSE_MESSAGE, buffer, strlen(CLOSE_MESSAGE)) == 0) {
            printf("Closing connection...\n");
            break;
           }
           memset(buffer, 0, 1024);
           printf("Enter message for client: ");
           fgets(buffer, 1024, stdin);
           send(new_socket, buffer, strlen(buffer), 0);
           memset(buffer, 0, 1024);
       }        
    }
    close(new_socket);
    return 0;
}

