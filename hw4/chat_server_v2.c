#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
#include <poll.h>

#define MAX_CLIENTS 64
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

    struct pollfd fds[MAX_CLIENTS];
    int nfds = 1;

    fds[0].fd = listener;
    fds[0].events = POLLIN;
    char buf[256];

    int users[64];      // Mang socket client da dang nhap
    char *user_ids[64]; // Mang luu tru id cua client da dang nhap
    int num_users = 0;  // So client da dang nhap


    while (1)
    {
        int ret = poll(fds, nfds, -1);
        if(ret<0){
            perror("poll() failed");
            break;
        }
        if(fds[0].revents & POLLIN){
            int client = accept(listener, NULL, NULL);
            if(nfds< MAX_CLIENTS){
                printf("New client connected: %d\n", client);
                fds[nfds].fd = client;
                fds[nfds].events = POLLIN;
                nfds++;
            }
        }
        for (int i = 1; i < nfds; i++){
            if(fds[i].revents & POLLIN){
                ret = recv(fds[i].fd, buf, sizeof(buf), 0);
                if(ret<=0){
                    printf("Client %d disconnected.\n", fds[i].fd);
                    close(fds[i].fd);
                    //TODO: xoa phan tu ra khoi mang
                    for (int k = 0; k < num_users; k++){
                        // xoa user ra khoi mang users, user_ids
                        if(users[k]==fds[i].fd){
                            if(k==num_users-1){
                                break;
                            }
                            for (int m = k; m < num_users - 1; m++)
                            {
                                users[m] = users[m + 1];
                                user_ids[m] = user_ids[m + 1];
                            }
                        }
                    }
                    num_users--;
                    
                    if(i < nfds-1){
                        fds[i] = fds[nfds - 1];
                    }
                    nfds--;
                    i--;
                }
                else
                {
                    buf[ret] = 0;
                    printf("Received from %d: %s\n", fds[i].fd, buf);
                
                     // Kiem tra trang thai dang nhap cua client
                    int client = fds[i].fd;

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
                        char cmd[32], id[32], tmp[32];
                        ret = sscanf(buf, "%s%s%s", cmd, id, tmp);
                        if (ret == 2)
                        {
                            if (strcmp(cmd, "client_id:") == 0)
                            {

                                int k = 0;
                                for (; k < num_users; k++)
                                    if (strcmp(user_ids[k], id) == 0) break;
                                    
                                if (k < num_users)
                                {
                                    char *msg = "ID da ton tai. Yeu cau nhap lai.\n";
                                    send(client, msg, strlen(msg), 0);
                                }
                                else
                                {
                                    users[num_users] = client;
                                    user_ids[num_users] = malloc(strlen(id) + 1);
                                    strcpy(user_ids[num_users], id);
                                    num_users++;
                                }                                    
                                char *msg = "Dung cu phap. Gui tin nhan.\n";
                                send(client, msg, strlen(msg), 0);
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
                        char sendbuf[512];
                        char * token = strtok(buf, ":");
                        if(strcmp(token,"[ca nhan]")!=0){
                            sprintf(sendbuf, "%s: %s", user_ids[j], buf);

                            for (int k = 0; k < num_users; k++)
                                if (users[k] != client)
                                    send(users[k], sendbuf, strlen(sendbuf), 0);

                        }
                    }   
                }
            }
        }
    }
    return 1;
}