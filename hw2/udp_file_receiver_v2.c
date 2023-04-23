#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_SENDERS 10

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

    // Initialize sender addresses array
    struct sockaddr_in sender_addrs[MAX_SENDERS];
    memset(sender_addrs, 0, sizeof(sender_addrs));
    int num_senders = 0;

    FILE *f = fopen("./txt/udp_v2.txt", "wb");

    int ret;
    // Receive data from senders
    char buffer[2048];
    int nread;
    while ((nread = recvfrom(receiver, buffer,2048, 0, NULL, NULL)) > 0) {
        // Check if sender address is already in the array
        struct sockaddr_in sender_addr;
        memcpy(&sender_addr, &buffer, sizeof(sender_addr));
        int sender_idx = -1;
        for (int i = 0; i < num_senders; i++) {
            if (sender_addrs[i].sin_addr.s_addr == sender_addr.sin_addr.s_addr &&
                sender_addrs[i].sin_port == sender_addr.sin_port) {
                sender_idx = i;
                break;
            }
        }
        if (sender_idx == -1) {
            // Add new sender address to the array
            if (num_senders == MAX_SENDERS) {
                fprintf(stderr, "Max number of senders reached\n");
                continue;
            }
            sender_addrs[num_senders++] = sender_addr;
            sender_idx = num_senders - 1;
        }
        puts(buffer);
        // Write data to file or stdout, depending on sender index
        if (sender_idx == 0) {
            // First sender writes to stdout
            fwrite(buffer + sizeof(sender_addr), 1, nread - sizeof(sender_addr), stdout);
        } else {
            // Other senders write to file
        
            fwrite(buffer + sizeof(sender_addr), 1, nread - sizeof(sender_addr), f);
            fclose(f);
        }
    }

    
   
    return 1;
}