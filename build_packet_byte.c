#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "parse.h"
#include "question.h"
#include "structure_definitions.h"

#include "question.c"
#include "query_header_builder.c"


uint16_t* build_dns_packet(uint16_t* dns_header, uint16_t* dns_question,int quesiton_len){
    
    // printf("%d",quesiton_len);
    byte* dns_packet = (byte*)malloc(sizeof(uint16_t)*(6)+sizeof(quesiton_len));
    
    int cursor = 0;
    for(int i = 0;i<6;i++)
    {
        dns_packet[cursor++] = ((dns_header[i] >> 8) & 0xFF);//msb
        dns_packet[cursor++] = ((dns_header[i]) & 0xFF); //lsb
    }
    for(int j = 0;j<quesiton_len;j++)
    {
        dns_packet[cursor++] = dns_question[j];
    }
    for (size_t i = 0; i < quesiton_len+12; ++i) {
        printf("%04X ", dns_packet[i]);
    }
}

// int main(int argc, char const *argv[])
// {
    
//     char url[] = "www.google.com";
//     int pair_count = 0;
//     LL_PAIRS* pairs = parse(url, strlen(url), &pair_count);
//     int question_size = 0;
//     uint16_t* quesiton_segment = build_question(pairs,pair_count,&question_size);
//     uint16_t* header_segment = build_q_header(0xDB42,0,1);
//     build_dns_packet(header_segment,quesiton_segment,question_size);

//     return 0;
// }
