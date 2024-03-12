#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "parse.h"


int main() {
    char url[254] = "www.google.com";
    /**
     * The domain you want to resolve is stored in url.
     * The length of the entire domain must be less than 254. 
     * 253 is the max length allowed for domain names.
    */
    int len = strlen(url);
}