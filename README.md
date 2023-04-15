Run sv_client.c and sv_server.c
```
gcc sv_client.c -o sv_client
./sv_client 127.0.0.1 9000

gcc sv_server.c -o sv_server
./sv_server
```

Run tcp_client.c and tcp_server.c
```
gcc tcp_client.c -o tcp_client
./tcp_client 127.0.0.1 9000

gcc tcp_server.c -o tcp_server
./tcp_server 9000 ./txt/hello.txt ./txt/log_tcp_server.txt
```