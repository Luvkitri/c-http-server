#include <arpa/inet.h>
#include <complex.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define REQUEST_SIZE 1024

typedef struct server {
  int socketfd;
  struct sockaddr_in address;
  int backlog;
} server_t;

typedef struct request {
  char method[10];
  char route[100];
} request_t;

server_t create_server(uint16_t port, uint32_t address);
void bind_socket(server_t *server_data);
void start_listening(server_t *server_data);
void get_server_info(server_t *server_data);
request_t parse_request_data(char *request_data);

int main() {
  server_t server_data = create_server(8080, INADDR_LOOPBACK);

  bind_socket(&server_data);
  start_listening(&server_data);
  get_server_info(&server_data);

  char *request_data;

  while (1) {

    int client_socketfd = accept(server_data.socketfd, NULL, NULL);

    if (client_socketfd == -1) {
      printf("ERROR while accepting client socket!\n errno: %d\n%s\n", errno,
             strerror(errno));
      exit(EXIT_FAILURE);
    }

    request_data = malloc(REQUEST_SIZE * sizeof(char));
    int read_value = read(client_socketfd, request_data, REQUEST_SIZE);

    if (read_value == -1) {
      printf("ERROR while reading socket data!\n errno: %d\n%s\n", errno,
             strerror(errno));
      exit(EXIT_FAILURE);
    }

    request_t req = parse_request_data(request_data);
    free(request_data);

    printf("%s %s\n", req.method, req.route);
  };

  return 0;
}

server_t create_server(uint16_t port, uint32_t address) {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd == -1) {
    printf("ERROR while creating a socket!\n errno: %d\n%s\n", errno,
           strerror(errno));
    exit(EXIT_FAILURE);
  };

  struct sockaddr_in socket_address = {AF_INET, htons(port), htonl(address)};
  server_t server_data = {socketfd, socket_address};

  return server_data;
}

void bind_socket(server_t *server_data) {
  int bind_value =
      bind(server_data->socketfd, (struct sockaddr *)&server_data->address,
           sizeof(server_data->address));
  if (bind_value == -1) {
    printf("ERROR while binding a socket!\n errno: %d\n%s\n", errno,
           strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void start_listening(server_t *server_data) {
  int listen_value = listen(server_data->socketfd, server_data->backlog);
  if (listen_value == -1) {
    printf("ERROR while trying to listen for socket connections!\n errno: "
           "%d\n%s\n",
           errno, strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void get_server_info(server_t *server_data) {
  char host_buffer[NI_MAXHOST], service_buffer[NI_MAXSERV];
  int name_info_value = getnameinfo(
      (struct sockaddr *)&server_data->address, sizeof(server_data->address),
      host_buffer, sizeof(host_buffer), service_buffer, sizeof(service_buffer),
      NI_NUMERICHOST | NI_NUMERICSERV);

  if (name_info_value != 0) {
    printf(
        "ERROR while trying to get server socket information!\n Error: %d %s\n",
        name_info_value, gai_strerror(name_info_value));
    exit(EXIT_FAILURE);
  }

  printf("Server is listening on http://%s:%s\n", host_buffer, service_buffer);
}

request_t parse_request_data(char *request_data) {
  request_t req;
  int matched_no = sscanf(request_data, "%s %s", req.method, req.route);

  if (matched_no != 2) {
    printf("ERROR while parsing a request!\nerrno: %d\n%s\n", errno,
           strerror(errno));
    exit(EXIT_FAILURE);
  }

  return req;
}
