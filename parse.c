#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"

LL_PAIRS* parse(char url[], int len, int* arr_length) {
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
            free_pairs(pairs, loop_index);
            return NULL;
        }
        pairs[loop_index].len = strlen(token);
        token = strtok(NULL, ".");
    }
    return pairs;
}

void free_pairs(LL_PAIRS* pairs, int count) {
    for (int i = 0; i < count; i++) {
        free(pairs[i].label);
    }
    free(pairs);
}

int main() {
    char url[] = "www.google.com";
    int pair_count = 0;
    LL_PAIRS* pairs = parse(url, strlen(url), &pair_count);
}