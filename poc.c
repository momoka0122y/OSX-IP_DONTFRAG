#define __APPLE_USE_RFC_3542

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int setdonotfragment(int sock, int addr_family) {
  extern int errno;

  if (addr_family == AF_INET6) {
    int val = 1;
    if (setsockopt(sock, IPPROTO_IPV6, IPV6_DONTFRAG, &val, sizeof(val)) != 0) {
      printf("%s at setsockopt IPV6_DONTFRAG\n", strerror(errno));
    }

    int v6_only = -1;
    socklen_t v6_only_len = sizeof(v6_only);
    if (getsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &v6_only, &v6_only_len) != 0) {
      printf("%s at getsockopt IPV6_V6ONLY\n", strerror(errno));
    }

    if (v6_only) {
      return 0;
    }
  }
  int val = 1;
  if (setsockopt(sock, IPPROTO_IP, IP_DONTFRAG, &val, sizeof(val)) != 0) {
    printf("%s at setsockopt IP_DONTFRAG\n", strerror(errno));
  }
  return 0;
}

int main() {
  int sock;

  // dual stack ipv6
  sock = socket(AF_INET6, SOCK_STREAM, 0);
  if (sock < 0) {
    printf("socket failed\n");
    return 1;
  }
  int mode = 0;
  setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&mode, sizeof(mode));
  printf("- dualstack socket\n");
  int rv = setdonotfragment(sock, AF_INET6);
  printf("\n");

  // single stack ipv6
  sock = socket(AF_INET6, SOCK_STREAM, 0);
  if (sock < 0) {
    printf("socket failed\n");
    return 1;
  }
  mode = 1;
  setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&mode, sizeof(mode));
  printf("- IPv6 socket\n");
  rv = setdonotfragment(sock, AF_INET6);
  printf("\n");

  // single stack ipv4
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    printf("socket failed\n");
    return 1;
  }
  printf("- IPv4 socket\n");
  rv = setdonotfragment(sock, AF_INET);
  printf("\n");
  return 0;
}

/*
output of this function

- dualstack socket
Invalid argument at setsockopt IP_DONTFRAG

- IPv6 socket

- IPv4 socket

*/