#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "structure_definitions.h"
#include "query_header_builder.h"

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



