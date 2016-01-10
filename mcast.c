#include <arpa/inet.h>
#include "network.h"

int main(int argc, char** argv)
{
  int sock = BuildSocket();
  struct sockaddr_in addr = BuildAddress();
  Send(addr, sock);
}

