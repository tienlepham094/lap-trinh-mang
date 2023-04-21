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
    if(argc != 4){
        printf("Sai tham so.\n");
        return 1;
    }
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    char buf[2048];
    FILE *f = fopen(argv[3], "rb");
    int res;
    while (!feof(f))
    {
        res = fread(buf, 1, sizeof(buf), f);
        if (res <= 0)
            break;
        
        int ret = sendto(sender, buf, strlen(buf), 0,
                (struct sockaddr *)&addr, sizeof(addr));
        printf("%d bytes sent\n", ret);
        
    }
    fclose(f);


}