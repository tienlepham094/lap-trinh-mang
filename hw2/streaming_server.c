#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(){
     int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind() failed");
        return 1;        
    }
    if (listen(listener, 5))
    {
        perror("listen() failed");
        return 1;
    }
    // khai bao socket
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    int client = accept(listener, 
        (struct sockaddr *)&client_addr, 
        &client_addr_len);
     int count = 0;
    char buf[20], receive[21];
    memset(buf, 'x', 9);
    memset(buf + 9, 0, 11);
    while (1)
    {
        memset(receive, 0, 21);
        int bytes_received = recv(client, receive, 20, 0);
        receive[bytes_received] = '\0';
        if (bytes_received == -1)
        {
            perror("recv() failed");
            exit(EXIT_FAILURE);
        }
        else if (bytes_received == 0)
        {
            printf("Count: %d\n", count);
            break;
        }
        strncat(buf, receive, 10);
        if (strstr(buf, "0123456789") != NULL)
        {
            count++;
        }
        strcpy(buf, buf + 10);
        strcat(buf, receive + 10);
        if (strstr(buf, "0123456789") != NULL)
        {
            count++;
        }
        strcpy(buf, buf + 10);
    }
    close(client);
    close(listener);
    return 0;
        
    }

