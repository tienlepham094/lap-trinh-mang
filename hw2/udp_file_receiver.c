#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Sai tham so.\n");
        return 1;
    }
    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    bind(receiver, (struct sockaddr *)&addr, sizeof(addr));

    char buf[2048];
    FILE *f = fopen("./txt/write.txt", "wb");
   
    int ret = recvfrom(receiver, buf, sizeof(buf), 0,
        NULL, NULL);
    if (ret == -1) {
        printf("recvfrom() failed\n");
        return 0;
    }

    buf[ret] = 0;
    printf("%d bytes received: %s\n", ret, buf);
    fwrite(buf, 1, ret, f);
    fclose(f);
    return 1;
}