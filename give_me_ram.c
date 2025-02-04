#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "give_me_ram.h"

/** @brief global heap */
gmr_heap_t* g_gmr_heap = NULL;


void* give_me_ram(size_t size) {
    if (g_gmr_heap == NULL) {
        // 
    }


    return NULL;
}

void free_me_ram(void* ptr) {
    return;
}
