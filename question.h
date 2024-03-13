#ifndef DNS_QUESTION_BUILDER_H
#define DNS_QUESTION_BUILDER_H

#include "parse.h"


typedef struct pair
{
    uint16_t label_Len;
    uint16_t *label_Data;
}LabelPair;


typedef struct Question
{
    uint16_t QNAME;
    uint16_t QTYPE;
    uint16_t QCLASS; 
}QUESTION;

typedef uint8_t byte;


/**
 * @brief Builds a DNS question from raw data in the format of a list of labels.
 *
 * The labels are represented by LL_PAIRS, where each label has a length and the actual data.
 * The resulting DNS question includes the QNAME, QTYPE, and QCLASS fields.
 *
 * @param raw_data An array of LL_PAIRS representing the labels in the DNS question.
 * @param data_len The number of elements in the raw_data array.
 * @return A dynamically allocated array containing the DNS question.
 *         The caller is responsible for freeing the memory when done using it.
 */
uint16_t* build_question(LL_PAIRS* raw_data, int data_len,  int* question_len);


#endif // DNS_QUESTION_BUILDER_H

