run udp_file_sender.c and upd_file_receiver.c
```
gcc udp_file_sender.c -o udp_file_sender
./udp_file_sender 127.0.0.1 9000 ./txt/read.txt

gcc udp_file_receiver.c -o udp_file_receiver
./udp_file_receiver 9000
```