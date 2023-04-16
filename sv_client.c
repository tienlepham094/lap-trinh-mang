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
    
    if (res<0)
    {
        printf("Khong ket noi duoc voi server\n");
        return -1;
    }
    
    // gui du lieu den server
    while(1){
        char name[256], mssv[256], dob[256], point[256];
        printf("\nNhap MSSV: ");
        fgets(mssv, sizeof(mssv), stdin);
        mssv[strlen(mssv)-1] = ' ';

        printf("\nNhap ten sv: ");
        fgets(name, sizeof(name), stdin);
        name[strlen(name)-1] = ' ';

        printf("\nNhap ngay sinh: ");
        fgets(dob, sizeof(dob), stdin);
        dob[strlen(dob)-1] = ' ';

        printf("\nNhap diem TB: ");
        fgets(point, sizeof(point), stdin);

        char buffer[512];
        strcpy(buffer, mssv);
        strcat(buffer, name);
        strcat(buffer, dob);
        strcat(buffer, point);
        send(client, buffer, strlen(buffer), 0);

        if(strncmp(mssv,"exit", 4) == 0) 
            break;
    }
    // ket thuc, dong socket
    close(client);
    return 0;
}