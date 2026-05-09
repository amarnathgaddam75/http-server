#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>

#include "http_parser.h"
#include "file_handler.h"

//
void *handle_client(void *arg)
{
    int client_fd = *(int *)arg;
    free(arg);  // free allocated memory

    char buffer[2048] = {0};

    int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

    if (bytes_read <= 0)
    {
        close(client_fd);
        return NULL;
    }

    char method[10], path[100];
    parse_request(buffer, method, path);

    printf("\n--- Request ---\n");
    printf("Method: %s\nPath: %s\n", method, path);

    if (strcmp(method, "GET") == 0)
    {
        handle_file_request(client_fd, path);
    }
    else
    {
        char *response =
            "HTTP/1.1 405 Method Not Allowed\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "Method Not Allowed";

        write(client_fd, response, strlen(response));
    }

    close(client_fd);
    return NULL;
}

int main()
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Address setup
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 5) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Multithreaded server running on port 8080...\n");

    while (1)
    {
        int *client_fd = malloc(sizeof(int));

        *client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        if (*client_fd < 0)
        {
            perror("Accept failed");
            free(client_fd);
            continue;
        }

        pthread_t thread_id;

        // Create thread
        if (pthread_create(&thread_id, NULL, handle_client, client_fd) != 0)
        {
            perror("Thread creation failed");
            free(client_fd);
            continue;
        }

        // Detach thread (auto cleanup)
        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}