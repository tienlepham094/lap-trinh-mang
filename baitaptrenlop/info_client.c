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
   
    int res = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    
    if (res<0)
    {
        printf("Khong ket noi duoc voi server\n");
        return -1;
    }
    char mt[64];
    int sodia;
    int i;
    char buf[256];
    int pos=0;

    printf("Nhap thong tin may tinh \n");
    printf("Nhap ten mt: ");
    scanf("%s", mt);
    getchar();
    strcpy(buf, mt);
    pos = pos + strlen(mt);
    buf[pos] = 0;
    pos++;

    printf("Nhap so o dia: ");
    scanf("%d", &sodia);
    getchar();
    


    char drive_letter;
    unsigned short drive_size;
    for (i = 0; i < sodia; i++)
    {
        printf("Nhap ten o dia %d: ", i + 1);
        scanf("%c", &drive_letter);
        getchar();
        buf[pos] = drive_letter;
        pos++;

        printf("Nhap dung luong o dia %d: ", i + 1);
        scanf("%hd", &drive_size);
        getchar();
        memcpy(buf + pos, &drive_size, sizeof(drive_size));
        pos += sizeof(drive_size);
    }
    printf("Buffer size: %d", pos);
    send(client, buf, pos, 0);
    close(client);
    return 0;
}