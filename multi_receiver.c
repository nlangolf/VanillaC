#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, const char *argv[])
{
    const char *group = argv[1];

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    int reuse = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) == -1) {
        fprintf(stderr, "setsockopt: %d\n", errno);
        return 1;
    }

    /* construct a multicast address structure */
    struct sockaddr_in mc_addr;
    memset(&mc_addr, 0, sizeof(mc_addr));
    mc_addr.sin_family = AF_INET;
    mc_addr.sin_addr.s_addr = inet_addr(group);
    mc_addr.sin_port = htons(6000);//19283);

    if (bind(s, (struct sockaddr*) &mc_addr, sizeof(mc_addr)) == -1) {
        fprintf(stderr, "bind: %d\n", errno);
        return 1;
    }

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group);
    mreq.imr_interface.s_addr = INADDR_ANY;
    setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

    char buf[1024];
    int n = 0;
    while ((n = read(s, buf, 1024)) > 0) {
        printf("group %s fd %d len %d: %.*s\n", group, s, n, n, buf);
    }
}
