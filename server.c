#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
  // Socket file descriptor
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);

  if (socketfd == -1) {
    printf("ERROR while creating a socket");
    return -1;
  }

  struct sockaddr_in socket_address = { AF_INET, htons(8080), 0};

  int bind_value = bind(socketfd, &socket_address, sizeof(socket_address));

  if (bind_value == -1) {
    printf("ERROR while binding a socket to an address");
    return -1;
  }
 

  return 0;
}
