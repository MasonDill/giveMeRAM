/**
 * @file gmr_platform.h
 * @brief Platform-specific memory operations
 * @author mason dill
 * @date 2025-01-25
 */

#ifndef GMR_PLATFORM_H
#define GMR_PLATFORM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
    #include <windows.h>
    #include <memoryapi.h>
#else
    #error "Unsupported platform"
#endif

/**
 * @brief Reserve a block of virtual memory
 * 
 * @param size Size of memory block to reserve
 * @return void* Pointer to reserved memory, or NULL if failed
 */
void* platform_reserve_memory(size_t size);

/**
 * @brief Commit previously reserved memory for use
 * 
 * @param ptr Pointer to reserved memory
 * @param size Size of memory to commit
 * @return void* Pointer to committed memory, or NULL if failed
 */
void* platform_commit_memory(void* ptr, size_t size);

/**
 * @brief Decommit memory but keep address space reserved
 * 
 * @param ptr Pointer to committed memory
 * @param size Size of memory to decommit
 * @return bool True if successful
 */
bool platform_decommit_memory(void* ptr, size_t size);

/**
 * @brief Free reserved memory
 * 
 * @param ptr Pointer to memory
 * @return bool True if successful
 */
bool platform_free_memory(void* ptr);

#endif // GMR_PLATFORM_H 