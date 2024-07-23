#include <arpa/inet.h>
#include <complex.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

typedef struct server {
  int socketfd;
  struct sockaddr_in address;
  int backlog;
} server_t;

void start_server(server_t *server_data);
server_t create_server();

int main() {
  server_t server_data = create_server();

  int bind_value =
      bind(server_data.socketfd, (struct sockaddr *)&server_data.address,
           sizeof(server_data.address));
  if (bind_value == -1) {
    printf("ERROR while binding a socket!\n errno: %d %s\n", errno,
           strerror(errno));
    exit(EXIT_FAILURE);
  }

  return 0;
}

server_t create_server() {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd == -1) {
    printf("ERROR while creating a socket!\n errno: %d %s\n", errno,
           strerror(errno));
    exit(EXIT_FAILURE);
  };

  struct sockaddr_in address = {AF_INET, htons(8080), htonl(INADDR_LOOPBACK)};

  struct server server_data = {socketfd, address};
  return server_data;
}

void start_server(server_t *server_data) {}
