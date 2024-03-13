#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdint.h>
#include "udp_implementation.h"

uint8_t* send_and_receive_dns(uint16_t* packet, int len) {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    const uint16_t *message = packet;
    int bytes_sent = sendto(sockfd, message, len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bytes_sent == SOCKET_ERROR) {
        printf("sendto failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    printf("Sent %d bytes to %s:%d\n", bytes_sent, SERVER_IP, SERVER_PORT);

    uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t)*512);
    int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
    if (bytes_received == SOCKET_ERROR) {
        printf("recvfrom failed: %d\n", WSAGetLastError());
    } else {
        printf("Received %d bytes from server: %s\n", bytes_received, buffer);
    }

    closesocket(sockfd);
    WSACleanup();

    return buffer;
}

