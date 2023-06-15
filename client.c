#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define CLOSE_MESSAGE "exit"

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char message[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    valread = read(sock, buffer, 1024);
    printf("%s", buffer);
    memset(buffer, 0, 1024);
    while (1) {
        printf("Enter message: ");
        fgets(message, 1024, stdin);
        send(sock, message, strlen(message), 0);
        if (strncmp(CLOSE_MESSAGE, message, strlen(CLOSE_MESSAGE)) == 0) {
            printf("Exiting...\n");
            break;
        }
        memset(message, 0, 1024);
        valread = read(sock, buffer, 1024);
        printf("From Server : %s", buffer);
        memset(buffer, 0, 1024);
    }

    return 0;
}


