
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Question.h"
#include "parse.h"

#include "parse.c"



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





