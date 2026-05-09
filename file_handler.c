#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "file_handler.h"

void handle_file_request(int client_fd, const char *path)
{
    char file_name[256];

    // Map "/" to index.html
    if (strcmp(path, "/") == 0)
    {
        strcpy(file_name, "index.html");
    }
    else
    {
        snprintf(file_name, sizeof(file_name), "%s", path + 1);
    }

    FILE *file = fopen(file_name, "rb");

    if (file == NULL)
    {
        char *not_found =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "404 File Not Found";

        write(client_fd, not_found, strlen(not_found));
        return;
    }

    // Basic content type detection
    char *content_type = "text/plain";

    if (strstr(file_name, ".html")) content_type = "text/html";
    else if (strstr(file_name, ".jpg")) content_type = "image/jpeg";
    else if (strstr(file_name, ".png")) content_type = "image/png";
    else if (strstr(file_name, ".css")) content_type = "text/css";
    else if (strstr(file_name, ".js")) content_type = "application/javascript";

    char header[512];
    sprintf(header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n\r\n",
        content_type);

    write(client_fd, header, strlen(header));

    char buffer[1024];
    int bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        write(client_fd, buffer, bytes);
    }

    fclose(file);
}
