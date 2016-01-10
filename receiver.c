#include <arpa/inet.h>
#include <stdio.h>
#include "network.h"

void ReceiveCallback(char* network_address, void* message)
{
  printf("recieved %s %s\n", network_address, message);
  // todo split up the network lib so sender doesn't need to implement this
}

int main(int argc, char** argv)
{
  int sock = BuildSocket();
  struct sockaddr_in addr = BuildAddress();
  Receive(addr, sock);
}

