#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include "network.h"

#define EXAMPLE_PORT 6000
#define EXAMPLE_GROUP "239.0.0.1"

void Send(struct sockaddr_in addr, int sock)
{
  unsigned int addrlen = sizeof(addr);
  addr.sin_addr.s_addr = inet_addr(EXAMPLE_GROUP);
  char message[50];
  while (1)
  {
    time_t t = time(0);
    sprintf(message, "time is %-24.24s", ctime(&t));
    printf("sending: %s\n", message);
    int cnt = sendto(sock, message, sizeof(message), 0,
    (struct sockaddr *) &addr, addrlen);
    if (cnt < 0)
    {
      perror("sendto");
      exit(1);
    }

    sleep(1); //TODO OBV pull this out of library
  }
}

void Receive(struct sockaddr_in addr, int sock)
{
  int s = socket(AF_INET, SOCK_DGRAM, 0);
  int reuse = 1;
  if (setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) == -1)
  {
    perror("sol socket");
    exit(1);
  }

  /* construct a multicast address structure */
  struct sockaddr_in mc_addr;
  memset(&mc_addr, 0, sizeof(mc_addr));
  mc_addr.sin_family = AF_INET;
  mc_addr.sin_addr.s_addr = inet_addr(EXAMPLE_GROUP);
  mc_addr.sin_port = htons(EXAMPLE_PORT);

  if (bind(s, (struct sockaddr*) &mc_addr, sizeof(mc_addr)) == -1)
  {
    perror("bind");
    exit(1);
  }

  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = inet_addr(EXAMPLE_GROUP);
  mreq.imr_interface.s_addr = INADDR_ANY;
  setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

  char message[50];

  while (1)
  {
    int n = 0;
    while ((n = read(s, message, 1024)) > 0)
    {
      //printf("group %s fd %d len %d: %.*s\n", EXAMPLE_GROUP, s, n, n, message);

      char* network_address = inet_ntoa(addr.sin_addr);
      ReceiveCallback(network_address, message);
    }
  }
}

int BuildSocket()
{
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0)
  {
    perror("socket");
    exit(1);
  }

  return sock;
}

struct sockaddr_in BuildAddress()
{
  struct sockaddr_in addr;
  bzero((char *)&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(EXAMPLE_PORT);
  return addr;
}
