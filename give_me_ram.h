/** 
* @file give_me_ram.h
* @brief malloc
* @author mason dill
* @date 2025-01-25
*/

#ifndef GIVE_ME_RAM_H
#define GIVE_ME_RAM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "platform/gmr_platform.h"

#define GMR_ENTRY_TERMINATOR NULL

typedef struct gmr_heap_entry gmr_heap_entry_t;

struct gmr_heap_entry {
    bool is_allocated;
    /** @brief pointer to the next entry */
    gmr_heap_entry_t* next_entry;
    /** @brief size of  */
    uint8_t entry_size;
    /** @brief NOT THE ACTUAL DATA - a pointer to the actual data
    * @details indirection allows for coalescing non-adjacent free blocks (at the cost of more memory)
    */
    uint8_t* pData; // indirection allows for 
};

/**
 * @brief Highest level wrapper for heap memory
 */
typedef struct {
    /** @brief pointer to the start of the heap */
    gmr_heap_entry_t* pStartEntry;
    /** @brief pointer to the end of the heap */
    gmr_heap_entry_t* pEndEntry;
    /** @brief size of the heap */
    size_t size;
} gmr_heap_t;


/**
 * @brief initialize the heap
 * 
 * @param heap 
 * @param start 
 * @param size 
 */
void gmr_heap_init();

/**
 * @brief allocate memory
 * 
 * @param size 
 * @return void* or NULL if allocation failed
 */ 
void* give_me_ram(size_t size);

/** 
 * @brief free memory
 * 
 * @param ptr 
 * @return void* or NULL if free failed
 */
void free_me_ram(void* ptr);

#endif
