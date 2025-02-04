#include "gmr_platform.h"

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)

void* platform_reserve_memory(size_t size) {
    return VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_NOACCESS);
}

void* platform_commit_memory(void* ptr, size_t size) {
    return VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
}

bool platform_decommit_memory(void* ptr, size_t size) {
    return VirtualFree(ptr, size, MEM_DECOMMIT) != 0;
}

bool platform_free_memory(void* ptr) {
    return VirtualFree(ptr, 0, MEM_RELEASE) != 0;
}

#endif 