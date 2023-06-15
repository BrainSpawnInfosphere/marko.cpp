

```c
#include <sys/socket.h>

// Create a socket
int sockfd = socket(AF_INET, SOCK_STREAM, 0); // TCP
int sockfd = socket(AF_UNIX, SOCK_STREAM, 0); // UDS
int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // UDP

if (sockfd < 0) {
    perror("socket");
    return 1;
}
```

The `struct sockaddr_un` structure is used to represent a Unix domain socket address. A Unix domain socket is a type of socket that is used for communication between processes on the same host. The `struct sockaddr_un` structure has the following members:

- `sun_family`: This member is a constant that specifies the address family. For Unix domain sockets, the address family is `AF_UNIX`.
- `sun_path`: This member is a pointer to a null-terminated string that contains the path to the socket file.


```c
#include <sys/un.h>

// Create a socket address for UDS
struct sockaddr_un addr;
memset(&addr, 0, sizeof(addr));
addr.sun_family = AF_UNIX;
strcpy(addr.sun_path, "/tmp/my_socket");

// Bind the socket to the address
if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    return 1;
}
```

```c
// Create a sockaddr_in structure for UDP
struct sockaddr_in addr;
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_port = htons(80);
addr.sin_addr.s_addr = inet_addr("127.0.0.1");

// Connect to the remote host
int err = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
if (err < 0) {
    perror("connect");
    return 1;
}
```

```c
// request to send datagram
// no need to specify server address in sendto
// connect stores the peers IP and port
sendto(sockfd, message, MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr));
sendto(sockfd, data, (size_t)size, 0, (struct sockaddr *)&addr, sizeof(addr));

// waiting for response
recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
recvfrom(sockfd, dst, (size_t)size, flags, (struct sockaddr *)&from_addr, &from_len);
```