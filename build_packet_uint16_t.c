#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#include "parse.h"
#include "question.h"
#include "structure_definitions.h"

#include "question.c"
#include "query_header_builder.c"

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
