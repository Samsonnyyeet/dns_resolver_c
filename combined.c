#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#define SERVER_IP "1.1.1.1"
#define SERVER_PORT 53 // port for DNS queries

enum query_op_code { 
    Standard, 
    Inverse, 
    Server_Status,  
    // Need to check if there are more query types
}; 

enum response_code { // 0 in queries
    No_Error,
    Format_Error,
    Server_Failure,
    Name_Error,
    Not_Implemented,
    Refused,
    // Need to check for more response types
};

typedef struct header_flags {
    bool qr;
    enum query_op_code query;
    bool aa;
    bool tc;
    bool rd;
    bool ra;
    int z;
    enum response_code rcode;
} HEAD_FLAGS;

typedef struct header {
    uint16_t id;
    HEAD_FLAGS flags;
    uint16_t qd_count;
    uint16_t an_count;
    uint16_t ns_count;
    uint16_t ar_count;
} HEADER;

typedef uint8_t byte;

typedef struct label_len_pairs {
    char *label;
    int len; // length of the label in bytes
} LL_PAIRS;

void free_label_len_pairs(LL_PAIRS* pairs, int count) {
    for (int i = 0; i < count; i++) {
        free(pairs[i].label);
    }
    free(pairs);
}

LL_PAIRS* parse_url(char url[], int len, int* arr_length) {
    // remove trailing http:// or https://
    // skipping this step for now assuming all input is in the following format - www.google.com

    int label_count = 0;
    char* temp = strchr(url, '.');
    while (temp!=NULL) {
        label_count++;
        temp = strchr(temp+1, '.');
    }
    label_count++; // number of labels

    LL_PAIRS *pairs = (LL_PAIRS*)malloc(label_count * sizeof(LL_PAIRS));
    *arr_length = label_count;

    // tokenize based on period
    char* token = strtok(url, ".");
    for (int loop_index = 0; token != NULL; loop_index++) {
        // printf("%s\n", token);
        pairs[loop_index].label = strdup(token);
        if (pairs[loop_index].label == NULL) {
            // handle memory allocation failure
            free_label_len_pairs(pairs, loop_index);
            return NULL;
        }
        pairs[loop_index].len = strlen(token);
        token = strtok(NULL, ".");
    }
    return pairs;
}

uint16_t* build_query_header(uint16_t id, int op_code, uint16_t qd_count) {
    uint16_t flags = 0x0000;

    switch (op_code)
    {
    case Standard:
        flags = flags | 0x0000;
        break;
    case Inverse:
        flags = flags | 0x0800;
        break;
    case Server_Status:
        flags = flags | 0x1000;
        break;
    default:
        break;
    }

    // set rd = 1
    flags = flags | 0x0100;

    uint16_t remaining_counts = 0x0000;

    uint16_t *final_q_head = (uint16_t*)malloc(sizeof(uint16_t)*6);

    final_q_head[0] = id;
    final_q_head[1] = flags;
    final_q_head[2] = qd_count;
    final_q_head[3] = remaining_counts;
    final_q_head[4] = remaining_counts;
    final_q_head[5] = remaining_counts;
    // for (size_t i = 0; i < 6; ++i) {
    //     printf("%04X ", final_q_head[i]);
    // }
    return final_q_head;
}

uint16_t* build_question(LL_PAIRS* raw_data,int data_len, int* question_len){

    //Question will be given in the format of 
    // 0x03 : label has length 3
    // 0x777777 : 3 octects that encode www

    int section_len = 0;
    for(int i = 0;i<data_len;i++)
    {
        section_len += raw_data[i].len + 1; 
    }

    //with section len we find the number of octets i.e sum of all lens +  number of segments (As each segments requires a count octet)

    uint16_t *dns_question = (uint16_t*)malloc((sizeof(byte)*(section_len + 5))); // section len is for QNAME;1 for null; 2 for QTYPE; 2 for QCLASS

    //Since we have found the number of octets, we can allocate the size of 8 * section_len
    
    int cursor = 0;
    //cursor indicates the location of dns_question we are populating


    for(int i = 0; i<data_len ; i++) // parsing through raw_data
    {
        char* label_data = raw_data[i].label;
        dns_question[cursor++] = (byte)raw_data[i].len; //adding the len octet

        for(int j = 0; j < raw_data[i].len;j++)
        {
            dns_question[cursor++] = (byte)label_data[j]; // adding each individual character's ascii value as octets
        }
    }
    //null
    dns_question[cursor++] = 0x00;

    // Defaulting QTYPE to 0x0001 (represents _host addresses_)
    dns_question[cursor++] = 0x00;
    dns_question[cursor++] = 0x01;

    // Defaulting QCLASS to 0x0001 (represents _internet addresses_)
    dns_question[cursor++] = 0x00;
    dns_question[cursor++] = 0x01;
    
    *question_len = (cursor);
    return dns_question;   
}

uint16_t *build_dns_query_packet(uint16_t *dns_header, uint16_t *dns_question, int quesiton_len, int *len)
{

    // printf("%d",quesiton_len);

    int combined_len = 6 + ceil(quesiton_len / 2);
    *len = combined_len;
    // printf("\n\n%d\n\n",combined_len);
    uint16_t *dns_packet = (uint16_t *)malloc(sizeof(uint16_t) * combined_len);

    int cursor = 5;

    memcpy(dns_packet, dns_header, sizeof(uint16_t) * 6);
    for (int j = 0; cursor < combined_len; cursor++, j += 2)
    {
        dns_packet[cursor] = (uint16_t)(dns_question[j] << 8 | dns_question[j + 1]);
    }

    return dns_packet;
}

uint16_t *generate_packet(char *url, int *len)
{
    int pair_count = 0;
    LL_PAIRS *pairs = parse_url(url, strlen(url), &pair_count);
    int question_size = 0;
    uint16_t *quesiton_segment = build_question(pairs, pair_count, &question_size);
    srand(time(0));
    uint16_t *header_segment = build_query_header(rand(), 0, 1);
    return build_dns_query_packet(header_segment, quesiton_segment, question_size, len);
}

uint8_t* send_and_receive_dns(uint16_t* packet, int len) {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return NULL;
    }

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        return NULL;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    const uint16_t *message = packet;
    int bytes_sent = sendto(sockfd, message, len*2, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)); // issue
    if (bytes_sent == SOCKET_ERROR) {
        printf("sendto failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return NULL;
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


int main() {
    char url[254] = "www.northeastern.edu";
    /**
     * The domain you want to resolve is stored in url.
     * The length of the entire domain must be less than 254. 
     * 253 is the max length allowed for domain names.
    */

    // create packet
    int len;
    uint16_t* packet = generate_packet(url, &len);
    
    uint8_t *response = send_and_receive_dns(packet, len);

    for (size_t i = 0; i < 1024; ++i) {
        printf("%04X ", response[i]);
    }
}