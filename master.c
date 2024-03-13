#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "parse.h"
#include "build_packet_uint16_t.c"
#include "udp_implementation.h"

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