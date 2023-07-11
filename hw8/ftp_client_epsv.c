#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main(){
    int ctrl_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in ctrl_addr;
    ctrl_addr.sin_family = AF_INET;
    ctrl_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ctrl_addr.sin_port = htons(21); 

    if (connect(ctrl_socket, (struct sockaddr *)&ctrl_addr, sizeof(ctrl_addr))) {
        perror("connect() failed");
        return 1;
    }

    char buf[2048];
    
    // Nhan xau chao tu server
    int len = recv(ctrl_socket, buf, sizeof(buf), 0);
    buf[len] = 0;
    // puts(buf);

    char username[64], password[64];
    while (1)
    {
        printf("Nhap username: ");
        scanf("%s", username);
        printf("Nhap password: ");
        scanf("%s", password);

        // Gui lenh USER
        sprintf(buf, "USER %s\r\n", username);
        send(ctrl_socket, buf, strlen(buf), 0);

        len = recv(ctrl_socket, buf, sizeof(buf), 0);
        buf[len] = 0;
        // puts(buf);

        // Gui lenh PASS
        sprintf(buf, "PASS %s\r\n", password);
        send(ctrl_socket, buf, strlen(buf), 0);

        len = recv(ctrl_socket, buf, sizeof(buf), 0);
        buf[len] = 0;
        // puts(buf);

        if (strncmp(buf, "230", 3) == 0)
        {
            printf("Dang nhap thanh cong.\n");
            break;
        }
        else
        {
            printf("Dang nhap that bai.\n");
        }
    }

    // Mo ket noi du lieu
    // Gui lenh EPSV
    send(ctrl_socket, "EPSV\r\n", 6, 0);
    len = recv(ctrl_socket, buf, sizeof(buf), 0);
    buf[len] = 0;
    puts(buf);

     // Xu ly ket qua lenh PASV
    char *pos1 = strchr(buf, '(') + 1;
    char *pos2 = strchr(pos1, ')');
    char temp[256];
    int n = pos2 - pos1;
    memcpy(temp, pos1, n);
    temp[n] = 0;

    char *p = strtok(temp, "|");
    int port = atoi(p);
    printf("%d", port);
        

    struct sockaddr_in data_addr;
    data_addr.sin_family = AF_INET;
    data_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    data_addr.sin_port = htons(port);

    int data_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (connect(data_socket, (struct sockaddr *)&data_addr, sizeof(data_addr))) {
            perror("connect() failed");
            return 1;
    }
    char *filename = "file.txt";
    sprintf(buf, "STOR %s\r\n", filename);
    send(ctrl_socket, buf, strlen(buf), 0);

    len = recv(ctrl_socket, buf, sizeof(buf), 0);
    buf[len] = 0;
    printf("%s", buf);
    FILE *file = fopen(filename, "rb");
        if (file == NULL)
        {
            printf("Khong the mo file.\n");
            return 1;
        }

        while (1)
        {
            int bytesRead = fread(buf, 1, sizeof(buf), file);
            if (bytesRead <= 0)
                break;

            send(data_socket, buf, bytesRead, 0);
        }

        fclose(file);
        close(data_socket);

    // Gui lenh QUIT
    send(ctrl_socket, "QUIT\r\n", 6, 0);
    len = recv(ctrl_socket, buf, sizeof(buf), 0);
    buf[len] = 0;
    puts(buf);

    // Ket thuc, dong socket
    close(ctrl_socket);

    return 0;
}