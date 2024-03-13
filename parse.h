#ifndef URL_PARSER_H
#define URL_PARSER_H

typedef struct label_len_pairs {
    char *label;
    int len; // length of the label in bytes
} LL_PAIRS;

/**
 * @brief Parses a URL string and returns an array of label-length pairs.
 *
 * @param url The URL string to be parsed.
 * @param len The length of the URL string.
 * @param arr_length A pointer to an integer that will hold the length of the returned array.
 * @return LL_PAIRS* A dynamically allocated array of label-length pairs. NULL on error.
 *
 * This function assumes that the input URL is in the format "www.example.com". It tokenizes
 * the URL based on the '.' character and creates an array of label-length pairs. The caller
 * is responsible for freeing the memory allocated for the array and its individual strings
 * using the free_pairs function.
 */
LL_PAIRS* parse(char url[], int len, int* arr_length);

/**
 * @brief Frees the memory allocated for an array of label-length pairs.
 *
 * @param pairs The array of label-length pairs to be freed.
 * @param count The number of elements in the array.
 *
 * This function frees the memory allocated for the labels and the array itself.
 */
void free_pairs(LL_PAIRS* pairs, int count);

#endif // URL_PARSER_H