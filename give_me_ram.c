#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "give_me_ram.h"

/** @brief global heap */
gmr_heap_t* g_gmr_heap = NULL;

void gmr_heap_init() {
    g_gmr_heap = (gmr_heap_t*)platform_reserve_memory(sizeof(gmr_heap_t));
    if (g_gmr_heap == NULL) {
        return;
    }
    g_gmr_heap = platform_commit_memory(g_gmr_heap, sizeof(gmr_heap_t));
    if (g_gmr_heap == NULL) {
        return;
    }
    
    uint8_t* start = (uint8_t*)platform_reserve_memory(sizeof(gmr_heap_entry_t));
    if (start == NULL) {
        return;
    }
    start = platform_commit_memory(start, sizeof(gmr_heap_entry_t));
    if (start == NULL) {
        return;
    }

    g_gmr_heap->pStartEntry = start;
    g_gmr_heap->size = 0;
    g_gmr_heap->pStartEntry->is_allocated = false;
    g_gmr_heap->pStartEntry->next_entry = GMR_ENTRY_TERMINATOR;
    g_gmr_heap->pStartEntry->entry_size = 0;
    g_gmr_heap->pStartEntry->pData = NULL;
    g_gmr_heap->pEndEntry = g_gmr_heap->pStartEntry;
}


void* give_me_ram(size_t size) {
    if (size == 0) {
        return NULL; // invalid size
    }

    if (g_gmr_heap == NULL) {
        return NULL; // heap not initialized
    }

    gmr_heap_entry_t* free_block = g_gmr_heap->pEndEntry;

    // Allocate the block
    free_block->is_allocated = true;
    free_block->entry_size = size;
    free_block->pData = (uint8_t*) platform_reserve_memory(size);
    if (free_block->pData == NULL) {
        return NULL;
    }
    free_block->pData = platform_commit_memory(free_block->pData, size);
    if (free_block->pData == NULL) {
        return NULL;
    }

    // update the free list
    if(free_block->next_entry == GMR_ENTRY_TERMINATOR) {

        // Add entry to the end of the free list
        free_block->next_entry = platform_reserve_memory(sizeof(gmr_heap_entry_t));
        if (free_block->next_entry == NULL) {
            return NULL;
        }
        free_block->next_entry = platform_commit_memory(free_block->next_entry, sizeof(gmr_heap_entry_t));
        if (free_block->next_entry == NULL) {
            return NULL;
        }
        free_block->next_entry->is_allocated = false;
        free_block->next_entry->next_entry = GMR_ENTRY_TERMINATOR;
        free_block->next_entry->entry_size = 0;
        free_block->next_entry->pData = NULL;
    }
    else{
        return NULL;
    }

    // update the end of the heap
    g_gmr_heap->pEndEntry = free_block->next_entry;

    return free_block->pData;
}

void free_me_ram(void* ptr) {
    if (ptr == NULL) {
        return;
    }
    if (g_gmr_heap == NULL) {
        return;
    }

    // find the block in the heap
    gmr_heap_entry_t* free_block = g_gmr_heap->pStartEntry;
    gmr_heap_entry_t* prev_block = NULL;
    while (free_block != GMR_ENTRY_TERMINATOR) {
        if (free_block->pData == ptr) {
            break;
        }
        prev_block = free_block;
        free_block = free_block->next_entry;
    }

    if (free_block == GMR_ENTRY_TERMINATOR) {
        return; // block not found
    }

    if (free_block->is_allocated == false) {
        return; // block is already free (this is the list head, address should be inaccessible)
    }

    if (free_block == g_gmr_heap->pStartEntry) {
        g_gmr_heap->pStartEntry = free_block->next_entry;
    }
    if (free_block == g_gmr_heap->pEndEntry) {
        g_gmr_heap->pEndEntry = prev_block;
    }

    // remove it from the heap list
    if (prev_block != NULL) {
        prev_block->next_entry = free_block->next_entry;
    }

    // Release the data
    if(platform_decommit_memory(free_block->pData, free_block->entry_size) == false) {
        return; // failed to decommit the block
    }

    if (platform_free_memory(free_block->pData) == false) {
        return; // failed to free the data
    }

    // Release the entry header
    if (platform_decommit_memory(free_block, sizeof(gmr_heap_entry_t)) == false) {
        return; // failed to decommit
    }
    
    if (platform_free_memory(free_block) == false) {
        return; // failed to free
    }

    return;
}

