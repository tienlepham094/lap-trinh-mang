run udp_file_sender.c and upd_file_receiver.c
```
gcc udp_file_sender.c -o udp_file_sender
./udp_file_sender 127.0.0.1 9000 ./txt/read.txt

gcc udp_file_receiver.c -o udp_file_receiver
./udp_file_receiver 9000
```

run streaming_client.c and streaming_server.c
```
gcc streaming_client.c -o streaming_client
./streaming_client

gcc streaming_server.c -o streaming_server
./streaming_server
```

run udp_file_sender_v2.c and udp_file_receiver_v2.c
```
gcc udp_file_sender_v2.c -o udp_file_sender_v2
./udp_file_sender_v2 127.0.0.1 9000 ./txt/read2.txt

gcc udp_file_receiver_v2.c -o udp_file_receiver_v2
./udp_file_receiver_v2 9000
```