void Send(struct sockaddr_in addr, int sock);
void Receive(struct sockaddr_in addr, int sock);
int BuildSocket();
struct sockaddr_in BuildAddress();
