#include <stdio.h>
#include <stdbool.h>
#include "give_me_ram.h"

#define FAILED_TO_ALLOCATE_MEMORY -1

int test_block_sz(size_t block_sz, bool allowed_expansion) {
    char* region = (char*)give_me_ram(block_sz);

    if (region == NULL) {
        printf("Failed to allocate memory for test_1024\n");
        return 1;
    }

    free_me_ram(region);

    return 0;
}

int main() {
    test_block_sz(64, false);
    return 0;
}
