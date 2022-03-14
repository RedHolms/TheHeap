// The Heap project
// Memory Allocator(memalloc)
// Free to use and distribute
/// Main Header

#ifndef f_mem_h_
#define f_mem_h_

#include <stddef.h>

#define __MEM_API

__MEM_API void*   memalloc    (size_t BLockSize);
__MEM_API void*   arralloc    (size_t ElSize, size_t ElCount);
__MEM_API void*   rememalloc  (void* OldBlock, size_t NewBlockSize);
__MEM_API void    free        (void* Block);

#endif