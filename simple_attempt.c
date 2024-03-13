#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 1024

void printbuf(char buf[DEFAULT_BUFLEN], int buflen) {
    printf("\n");
    for(int i = 0; i<buflen; i++) {
        printf("%c", buf[i]);
    }
    printf("\n");
}

int handle_request(char request[DEFAULT_BUFLEN]) 
{
    return 0;
}

int main() {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData); // makes a request for version 2.2 of winsock
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    struct addrinfo *result = NULL, *ptr = NULL, hints;
    ZeroMemory(&hints, sizeof (hints)); // zeroes out that block of memory ensuring nothing is stored there
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        goto close_wsa;
    }

    SOCKET ListenSocket = INVALID_SOCKET;

    // Create a SOCKET for the server to listen for client connections
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        goto free_addr;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        goto close_socket;
    }

    if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %ld\n", WSAGetLastError() );
        goto close_socket;
    }

    SOCKET ClientSocket;
    ClientSocket = INVALID_SOCKET;
    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        goto close_socket;
    }

    char recvbuf[DEFAULT_BUFLEN];
    int iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;
    int req_status;
    // Receive until the peer shuts down the connection
    // do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            printbuf(recvbuf, iResult);
            req_status = handle_request(recvbuf);
            // Echo the buffer back to the sender
            // iSendResult = send(ClientSocket, recvbuf, iResult, 0);
            // if (iSendResult == SOCKET_ERROR) {
            //     printf("send failed: %d\n", WSAGetLastError());
            //     goto close_client_socket;
            // }
            // printf("Bytes sent: %d\n", iSendResult);
        } else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed: %d\n", WSAGetLastError());
            goto close_client_socket;
        }

    // } while (iResult > 0);

    // shutdown the send half of the connection since no more data will be sent
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        goto close_client_socket;
    }

    // cleanup
    close_client_socket:
        closesocket(ClientSocket);
    close_socket:
        closesocket(ListenSocket);
    free_addr:
        freeaddrinfo(result);
    close_wsa:
        WSACleanup();

    return 0;
}