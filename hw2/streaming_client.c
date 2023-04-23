#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(){
     // khai bao socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // khai bao dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);
    if(inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0)  
    { 
        printf("Invalid address/ Address not supported \n"); 
        return -1; 
    }
    int res = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    
    if (res<0)
    {
        printf("Khong ket noi duoc voi server\n");
        return -1;
    }

    FILE *fp = fopen("./txt/streaming.txt", "r");
    if (fp == NULL)
    {
        perror("fopen() failed");
        exit(EXIT_FAILURE);
    }

    // Đọc file và gửi dữ liệu
    char buf[21];
    memset(buf, 0, 21);
    while (!feof(fp))
    {
        // Đọc dữ liệu từ file
        fgets(buf, 21, fp);

        // Gửi dữ liệu đến server
        if (send(client, buf, strlen(buf), 0) == -1)
        {
            perror("send() failed");
            exit(EXIT_FAILURE);
        }
    }
    printf("Data sent successfully!\n");
    fclose(fp);
    close(client);
    return 0;
}