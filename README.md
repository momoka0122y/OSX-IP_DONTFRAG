output of poc.c
```
- dualstack socket
Invalid argument at setsockopt IP_DONTFRAG

- IPv6 socket

- IPv4 socket
```

setsockopt(sock, IPPROTO_IP, IP_DONTFRAG, &val, sizeof(val)) 
return EINVAL for dualstack IPv4/IPv6 socket.

However for linux setsockopt(sock, IPPROTO_IP, IP_MTU_DISCOVER, &val, sizeof(val))
worked for dualstack IPv4/IPv6 socket.