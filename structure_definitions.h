#ifndef PACKET_STRUCTS_H
#define PACKET_STRUCTS_H

#include <stdint.h>
#include <stdbool.h>

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

#endif