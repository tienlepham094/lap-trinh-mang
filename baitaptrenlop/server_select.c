#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
int main() 
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9091);

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

    fd_set fdread, fdtest;
    // int clients[64];
    // int num_clients = 0;
    FD_ZERO(&fdread);
    FD_SET(listener, &fdread);
    char *msg = "Xin chao hien nay co ";
    char *msg2 = "clients dang ket noi";
    char buf[256];
    
    while (1)
    {
        fdtest = fdread;
       

        int ret = select(FD_SETSIZE, &fdtest, NULL, NULL, NULL);
        // select se xoa nhung skien ko chay ra khoi fdread 

        if(ret<0){
            perror("select() failed");
            break;
        }
        if(ret==0){
            printf("Timed out\n");
            // tra ve 0 khi het h
            continue;
        }

        for (int i = 0; i < FD_SETSIZE; i++){
            if(FD_ISSET(i, &fdtest)){
                // kiem tra su kien
                if(i==listener){
                    //day la socket server
                    // chap nhan ket noi
                    int client = accept(listener, NULL, NULL);

                    // kiem tra gioi han
                    if(client < FD_SETSIZE){
                        // hop le chua vuot qua gioi han
                        printf("New connected: %d\n", client);
                        FD_SET(client, &fdread);
                        char tmp[256];

                        send(client, msg, strlen(msg), 0);
                    }else{
                        // vuot qua gioi han
                        printf("Too many connections\n");
                        close(client);
                    }
                }else{
                    // day la socket client
                    // nhan du lieu
                    int ret = recv(i, buf, sizeof(buf), 0);
                    if(ret <=0){
                        close(i);
                        FD_CLR(i, &fdread);
                    }else{
                        buf[ret] = 0;
                        printf("Received from %d: %s\n", i, buf);
                    }
                }
            }
        }

    }
    
    close(listener);    

    return 0;
}