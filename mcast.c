#include <arpa/inet.h>
#include "network.h"

void ReceiveCallback(char* network_address, void* message)
{
 // todo split up the network lib so sender doesn't need to implement this
}

int main(int argc, char** argv)
{
  int sock = BuildSocket();
  struct sockaddr_in addr = BuildAddress();
  Send(addr, sock);
}

