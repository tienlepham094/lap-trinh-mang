#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
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

    if(client == -1){
        printf("accept() failed.\n");
        exit(1);
    }
    printf("New client connected!\n");
    // Truyen nhan du lieu

    char buf[256];
    int ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0)
        {
            printf("recv() failed.\n");
            exit(1);
        }
    
    buf[ret] = 0;
    printf("%d bytes received\n", ret);

    char mt[64];
    int i, pos =0;
   
    strcpy(mt, buf);
    printf("Computer name %s\n", mt);
    pos = strlen(mt) + 1;
    int num_drives = (ret - pos) / 3;
    for (i = 0; i < num_drives; i++){
        char drive_letter = buf[pos];
        pos++;
        unsigned short drive_size;
        memcpy(&drive_size, buf + pos, sizeof(drive_size));
        pos += sizeof(drive_size);
        printf("%c - %hd GB\n", drive_letter, drive_size);
    }
}
/**
 * Khai bao buffer nao do
 * Khai bao 9 ki tu dau tien bat ki
 * Su dung lenh recv de nhan luu tu ki tu thu 10
 * Dem xem sau day xh trong buffer bn lan?
 * Luu 9 bytes cua buffer hien tai vao dau cua no de cb cho lan nhan tiep theo
 * 
 */