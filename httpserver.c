#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

char *configuration(char file[])
{
    FILE *config = fopen("config.txt", "rb");
    char lines[100] = {0};
    while (fgets(lines, 100, config) != 0)
    {
        if (strstr(lines, file) != NULL)
        {
            char *r = strtok(lines, "=");
            r = strtok(NULL, "=");
            return r;
        }
    }
}

void setHttpHeader(char httpHeader[], char filename[])
{
    FILE *htmlData = fopen(filename, "rb");
    char line[100] = {0};
    char responseData[8000] = {0};
    while (fgets(line, 100, htmlData) != 0)
    {
        strcat(responseData, line);
    }
    strcat(httpHeader, responseData);
}

void getImage(int sockfd)
{
    FILE *ImageData = fopen("abc.jpg", "rb");

    while (1)
    {
        unsigned char buff[1024] = {0};
        int nread = fread(buff, 1, 1024, ImageData);
        if (nread > 0)
        {
            send(sockfd, buff, sizeof(buff), 0);
        }
        if (nread < 1024)
        {
            if (feof(ImageData))
            {
                break;
            }
        }
    }
    close(sockfd);
    shutdown(sockfd, SHUT_WR);
    sleep(2);
}

int main(void)
{
    char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    //******get Port number from config file****//

    int port = atoi(configuration("PORT"));
    char a[100];
    printf("Server listening on port %d\n", port);
    //******************************************

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(port);
    serveraddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    bind(serverSocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress));

    int listening = listen(serverSocket, 10);
    printf("Server is listening ");

    if (listening < 0)
    {
        printf("error the server is not listening ");
        return 1;
    }
    setHttpHeader(httpHeader, "index.html");
    printf("%s", httpHeader);
    char buffer[3000] = {0};
    while (1)
    {
        int clientSocket = accept(serverSocket, (struct sockaddr *)NULL, NULL);
        int valRead = read(clientSocket, buffer, 3000);

        printf("%s \n", buffer);
        if (strstr(buffer, ".jpg"))
        {
            getImage(clientSocket);
        }
        if (strstr(buffer, "aboutus"))
        {
            char AHeader[8000] = "HTTP/1.1 200 OK\r\n\n";
            setHttpHeader(AHeader, "aboutus.html");
            send(clientSocket, AHeader, sizeof(AHeader), 0);
            close(clientSocket);
            shutdown(clientSocket, SHUT_WR);
            sleep(2);
        }
        send(clientSocket, httpHeader, sizeof(httpHeader), 0);
        close(clientSocket);
    }
    return 0;
}