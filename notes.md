[Beej's Guide to Netowrking](https://beej.us/guide/bgnet/html//index.html#structs)

```c
// sa_data contains a destination address and port number for the socket.
// This is rather unwieldy since you don’t want to tediously pack the address
// in the sa_data by hand.
struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address
};

// To deal with struct sockaddr, programmers created a parallel
// structure: struct sockaddr_in (“in” for “Internet”) to be used with IPv4.
struct sockaddr_in {
    short int          sin_family;  // Address family, AF_INET
    unsigned short int sin_port;    // Port number
    struct in_addr     sin_addr;    // Internet address
    unsigned char      sin_zero[8]; // Same size as struct sockaddr
};

struct in_addr {
    uint32_t s_addr; // that's a 32-bit int (4 bytes)
};
```
```c
struct sockaddr_in sa; // IPv4

// See, inet_pton() returns -1 on error, or 0 if the address is messed up.
inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr)); // IPv4

char ip4[INET_ADDRSTRLEN];  // space to hold the IPv4 string

// “ntop” means “network to presentation”—you can call it
// “network to printable” if that’s easier to remember
inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);
printf("The IPv4 address is: %s\n", ip4);
```
