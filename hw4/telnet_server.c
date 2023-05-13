#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
#include <stdlib.h>
#include <stdbool.h>

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

    char buf[256];

    // read file line by line
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char *user[64], *password[64];
    int num = 0;
    fp = fopen("./file.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    // get user and password
    while ((read = getline(&line, &len, fp)) != -1) {
        // printf("Retrieved line of length %zu:\n", read);
        char u[32], p[32], tmp[32];
        int ret = sscanf(line, "%s%s%s", u, p, tmp);
      

        user[num] = malloc(strlen(u) + 1);
        strcpy(user[num], u);
        password[num] = malloc(strlen(p) + 1);
        strcpy(password[num], p);
        num++;
    }
    // for (int i = 0; i < num_users;i++){
    //     printf("%s %s\n", user[i], password[i]);
    // }
    fclose(fp);

    int num_users = 0;  // So client da dang nhap
    int users[64]; 

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
                        // Kiem tra trang thai dang nhap cua client
                        int client = i;

                        int j = 0;
                        for (; j < num_users; j++)
                            if (users[j] == client) {
                                // printf("%d", users[j]);
                                break;
                            }

                        if (j == num_users)
                        {
                            // Chua dang nhap
                            // Xu ly cu phap yeu cau dang nhap
                            char u[32], p[32], tmp[32];
                            ret = sscanf(buf, "%s%s%s", u, p, tmp);
                            if (ret == 2)
                            {
                                int check = 0;
                                for (int k = 0; k < num; k++)
                                {
                                    if(strcmp(user[k], u)==0 && strcmp(password[k],p)==0){
                                        check = 1;
                                        break;
                                    }
                                }
                                if(check==1){
                                    users[num_users] = client;
                                    char *msg = "Dung username va password.\n";
                                    send(client, msg, strlen(msg), 0);
                                    num_users++;
                                }
                                else
                                {
                                    char *msg = "Nhap sai. Yeu cau nhap lai.\n";
                                    send(client, msg, strlen(msg), 0);
                                }
                            }
                            else
                            {
                                char *msg = "Nhap sai. Yeu cau nhap lai.\n";
                                send(client, msg, strlen(msg), 0);
                            }
                        }
                        else
                        {
                            // Da dang nhap
                            char str[1024];
                            buf[strcspn(buf, "\n")] = 0;
                            strcpy(str, buf);
                            strcat(str, " > out.txt");

                            system(str);
                        }
                    }
                }
            }
        }

    }

    
    close(listener);    

    return 0;
}