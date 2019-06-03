#include <arpa/inet.h> 
#include <assert.h>
#include <netinet/ip.h>
#include <sys/socket.h>

#define guard_negone(x) if (x == -1) { return -1; }

// Create a TCP socket that is listening out of the box.
//
// Pass NULL to listen_addr to listen on all interfaces.
//
// The port must not be out of range and listen_backlog must be above zero.
int tcp_server(char *listen_addr, int port, int listen_backlog)
{
    int    sock;
    int    status;
    int    sockopt_val;
    struct sockaddr_in addr;

    // Sanity checks
    assert(port >= 0);
    assert(port <= 65535);
    assert(listen_backlog > 0);

    // Create the socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    guard_negone(sock);

    // SO_REUSEADDR
    sockopt_val = 1;
    status = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sockopt_val, sizeof(sockopt_val));
    guard_negone(status);

    // Bind to 0.0.0.0
	addr.sin_family      = AF_INET;
	addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = (listen_addr == NULL) ? INADDR_ANY : inet_addr(listen_addr);
	status = bind(sock, (struct sockaddr *) &addr, sizeof(addr));
    guard_negone(status);

    // Start listening
    status = listen(sock, listen_backlog);
    guard_negone(status);

    return sock;
}
