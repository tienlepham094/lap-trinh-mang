#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


void write_file(char *buf){
    FILE *f;
    f = fopen("./txt/sv_log.txt", "a"); 

    fwrite(buf, sizeof(char), strlen(buf), f);
    // fwrite("\n", 1,sizeof(char), f);
    fclose(f);
}

void write_log(char *buf, char *buf_ip){
    time_t now = time(NULL) ;
    struct tm tm_now ;
    localtime_r(&now, &tm_now) ;
    char buf_time[100] ;
    strftime(buf_time, sizeof(buf_time), "%Y-%m-%d %H:%M ", &tm_now);
    char buffer[2048];
    strcpy(buffer, buf_ip);
    strcat(buffer, buf_time);
    strcat(buffer, buf);
    puts(buffer);
    write_file(buffer);
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

    char buf[256];
    char buf_ip[256] = "127.0.0.1 ";
    FILE *f;
    f = fopen("./txt/sv_log.txt", "a");
    while (1)
    {   
        
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            printf("recv() failed.\n");
            exit(1);
        }
        if (ret < sizeof(buf))
            buf[ret] = 0;
        write_log(buf, buf_ip);
    }
    // fclose(f);
    close(client);
    close(listener);
}
