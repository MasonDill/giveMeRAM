#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "give_me_ram.h"

#define MAX_BLOCKS 16
#define FAILED_TO_ALLOCATE_MEMORY -1

typedef struct {
    void* ptr;
    size_t size;
    bool is_allocated;
} test_block_t;

// Helper function to fill a block with a pattern
void fill_block(char* ptr, size_t size, char pattern) {
    for (size_t i = 0; i < size; i++) {
        ptr[i] = (char)(pattern + i % 10);
    }
}

// Helper function to verify a block's pattern
bool verify_block(char* ptr, size_t size, char pattern) {
    for (size_t i = 0; i < size; i++) {
        if (ptr[i] != (char)(pattern + i % 10)) {
            printf("Block verification failed at index %zu. Expected %c, got %c\n",
                   i, (pattern + i % 10), ptr[i]);
            return false;
        }
    }
    return true;
}

int run_allocation_tests() {
    test_block_t blocks[MAX_BLOCKS] = {0};
    size_t block_sizes[] = {64, 128, 256, 512, 1024, 2048, 4096, 8192};
    const int num_sizes = sizeof(block_sizes) / sizeof(block_sizes[0]);
    
    printf("Starting allocation tests...\n");
    
    // First wave of allocations
    printf("\nPhase 1: Initial allocations\n");
    for (int i = 0; i < MAX_BLOCKS/2; i++) {
        size_t size = block_sizes[i % num_sizes];
        blocks[i].ptr = give_me_ram(size);
        blocks[i].size = size;
        
        if (blocks[i].ptr == NULL) {
            printf("Failed to allocate block %d of size %zu\n", i, size);
            return FAILED_TO_ALLOCATE_MEMORY;
        }
        
        blocks[i].is_allocated = true;
        fill_block(blocks[i].ptr, size, (char)('A' + i));
        printf("Allocated block %d: size=%zu, pattern=%c\n", i, size, 'A' + i);
    }
    
    // Verify first wave
    printf("\nVerifying first wave allocations...\n");
    for (int i = 0; i < MAX_BLOCKS/2; i++) {
        if (blocks[i].is_allocated) {
            if (!verify_block(blocks[i].ptr, blocks[i].size, (char)('A' + i))) {
                printf("Verification failed for block %d\n", i);
                return FAILED_TO_ALLOCATE_MEMORY;
            }
        }
    }
    
    // Free some blocks
    printf("\nPhase 2: Freeing alternate blocks...\n");
    for (int i = 0; i < MAX_BLOCKS/2; i += 2) {
        printf("Freeing block %d\n", i);
        free_me_ram(blocks[i].ptr);
        blocks[i].is_allocated = false;
    }
    
    // Second wave of allocations
    printf("\nPhase 3: Second wave allocations\n");
    for (int i = MAX_BLOCKS/2; i < MAX_BLOCKS; i++) {
        size_t size = block_sizes[(i + 3) % num_sizes]; // Offset to get different sizes
        blocks[i].ptr = give_me_ram(size);
        blocks[i].size = size;
        
        if (blocks[i].ptr == NULL) {
            printf("Failed to allocate block %d of size %zu\n", i, size);
            return FAILED_TO_ALLOCATE_MEMORY;
        }
        
        blocks[i].is_allocated = true;
        fill_block(blocks[i].ptr, size, (char)('a' + i));
        printf("Allocated block %d: size=%zu, pattern=%c\n", i, size, 'a' + i);
    }
    
    // Final verification of all blocks
    printf("\nFinal verification of all blocks...\n");
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i].is_allocated) {
            char pattern = (i < MAX_BLOCKS/2) ? ('A' + i) : ('a' + i);
            if (!verify_block(blocks[i].ptr, blocks[i].size, pattern)) {
                printf("Final verification failed for block %d\n", i);
                return FAILED_TO_ALLOCATE_MEMORY;
            }
            printf("Block %d verified successfully\n", i);
        }
    }
    
    // Cleanup
    printf("\nCleaning up...\n");
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i].is_allocated) {
            free_me_ram(blocks[i].ptr);
        }
    }
    
    printf("All tests completed successfully!\n");
    return 0;
}

int main() {
    printf("Initializing heap...\n");
    gmr_heap_init();
    
    int result = run_allocation_tests();
    if (result == 0) {
        printf("All memory tests passed!\n");
    } else {
        printf("Memory tests failed with code: %d\n", result);
    }
    
    return result;
}
