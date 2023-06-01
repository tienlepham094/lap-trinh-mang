#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include<time.h>

void process_request(int client, char *buf);
void signalHandler(int signo)
{
    int pid = wait(NULL);
    printf("Child %d terminated.\n", pid);
}

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
    addr.sin_port = htons(9090);

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

    signal(SIGCHLD, signalHandler);
    while(1)
    {
        if (fork() == 0)
        {
            char buf[256];
            int client = accept(listener, NULL, NULL);
            printf("New client connected: %d\n", client);
            while (1)
            {
                int ret = recv(client, buf, sizeof(buf), 0);
                if (ret <= 0){
                    printf("Client %d disconnected.\n", client);
                    break;
                }
                else{
                    buf[ret] = 0;
                    printf("Received from %d: %s\n", client, buf);
                    process_request(client, buf);
                }
            }
            close(client);
            exit(0);
        }
    }
}
void process_request(int client, char *buf){
    char title[32], format[32], tmp[66];
    int ret = sscanf(buf, "%s%s%s", title, format, tmp);
    if(ret==2){
        if(strcmp(title, "GET_TIME")==0){
        int check = 1;
            time_t now = time(NULL);
            struct tm *time_info = localtime(&now);
            char buffer[1024];
            if (strcmp(format, "dd/mm/yyyy") == 0)
            {
                strftime(buffer, 1024, "%d/%m/%Y\n", time_info);
                }
            else if (strcmp(format, "dd/mm/yy") == 0)
                {
                    strftime(buffer, 1024, "%d/%m/%y\n", time_info);
                }
            else if (strcmp(format, "mm/dd/yyyy") == 0)
                {
                    strftime(buffer, 1024, "%m/%d/%Y\n", time_info);
                }
            else if (strcmp(format, "mm/dd/yy") == 0)
                {
                    strftime(buffer, 1024, "%m/%d/%y\n", time_info);
                }
            else
                {
                    char *msg = "Nhap sai format ve thoi gian. Hay nhap lai.\n";
                    send(client, msg, strlen(msg), 0);
                    check = 0;
                }
            if(check){
                
                send(client, buffer, strlen(buffer), 0);
            }
        }
        else{
            char *msg = "Nhap sai cu phap. Hay nhap lai.\n";
            send(client, msg, strlen(msg), 0);
        }
    }
    else
        {
            char *msg = "Nhap sai cu phap. Hay nhap lai.\n";
            send(client, msg, strlen(msg), 0);
        }
}