#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

     if(argc != 3){
        printf("Sai tham so.\n");
        return 1;
    }
    char *ip = argv[1];
    char *host = argv[2];

    // khai bao socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // khai bao dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(host));

    if(inet_pton(AF_INET, ip, &addr.sin_addr) <= 0)  
    { 
        printf("Invalid address/ Address not supported \n"); 
        return -1; 
    }

    int res = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    
    if (res == -1)
    {
        printf("Cannot connect to server\n");
        return 1;
    }
    // nhan tin nhan tu server
    char buf_recv[2048];
    int len = recv(client, buf_recv, sizeof(buf_recv), 0);
    buf_recv[len] = 0;
    printf("Data received: %s\n", buf_recv);
    // gui du lieu den server
    while(1){
        char buf[256];
        printf("\nEnter string: ");
        fgets(buf, sizeof(buf), stdin);
        send(client, buf, strlen(buf), 0);

        if(strncmp(buf,"exit", 4) == 0) 
            break;
    }
    // ket thuc, dong socket
    close(client);
    return 0;
}