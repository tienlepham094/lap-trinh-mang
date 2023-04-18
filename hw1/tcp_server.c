#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool check_file(char *filename){
    if ( access(filename, F_OK) == 0){
        return true;
        }
    else {
        printf("File %s doesn't exist.", filename);
        return false;
    }
}

int main(int argc, char *argv[]){
    if(argc != 4){
        printf("Sai tham so.\n");
        return 1;
    }
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));


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

    // read from file
    char *file_read = argv[2];
    if(!check_file(file_read)){
        return 1;
    }
    FILE *f_read = fopen(file_read, "rb");
    int ret;
    char buf[2048];
    while (!feof(f_read))
    {
        ret = fread(buf, 1, sizeof(buf), f_read);
        if (ret < 0)
            break;
        if (ret < sizeof(buf))
            buf[ret] = 0;
        send(client, buf, ret, 0);
    }
    

    // write to file
    char *file_write = argv[3];
    if (!check_file(file_write))
        return 1;
    
    FILE *f_write = fopen(file_write, "wb");
    char buf2[2048];
    while (1)
    {
        ret = recv(client, buf2, sizeof(buf), 0);
        if (ret <= 0)
            break;
        if (ret < sizeof(buf2))
            buf2[ret] = 0;
        fwrite(buf2, 1, ret, f_write);
        puts(buf2);
    }
    fclose(f_read);
    fclose(f_write);
    close(client);
    close(listener);
}