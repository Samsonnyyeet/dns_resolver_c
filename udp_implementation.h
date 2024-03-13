#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdint.h>

#define SERVER_IP "1.1.1.1"
#define SERVER_PORT 53 // port for DNS queries

uint8_t* send_and_receive_dns(uint16_t* packet, int len);

#endif // UDP_CLIENT_H