// The Heap project
// Memory Allocator(memalloc)
// Free to use and distribute
/// Main Source

#include <errno.h>
#include <ntdef.h>
#include <assert.h>

/* Utils */
#define ClearMemory(p,s) memset(p,0,s)

/* Defines */
#define __API __declspec(dllexport)

#define HEAP_SIZE (size_t)(524288) // 512 KiB
#define HEAP_MAX (size_t)(67108864) // 64 MiB

#define HEAP_NOT_INIT (NULL)

typedef unsigned char byte_t;
typedef struct __BlockMeta {
   byte_t free;
   size_t size;
   struct __BlockMeta* next;
} _BlockMeta;
#define META_SIZE (sizeof(_BlockMeta))

NTSYSAPI ULONG NTAPI RtlGetProcessHeaps(
   IN ULONG    MaxNumberOfHeaps,
   OUT PVOID   *HeapArray
);

/* Globals */
byte_t* g__heap_base = HEAP_NOT_INIT;
byte_t* g_heap = HEAP_NOT_INIT;
_BlockMeta* g_lastBlock = NULL;

/* Code */
static void _init_heap() {
   RtlGetProcessHeaps(1, &g__heap_base);
   g_heap = g__heap_base;
}

static byte_t _check_heap_size(void* heap) {
   return (size_t)((byte_t*)heap - g__heap_base) < HEAP_MAX;
}

static _BlockMeta* _find_best_block(size_t size) {
   _BlockMeta* best_block = NULL;
   for (_BlockMeta* block = g_lastBlock; block != NULL; block = block->next) {
      if (best_block != NULL) {
         if (block->size >= size && block->free && block->size < best_block->size) { best_block = block; }
      } else if (block->size >= size && block->free) { best_block = block; }
   }
   if (best_block == NULL) {
      if (!_check_heap_size(g_heap + size)) { return NULL; }
      best_block = (_BlockMeta*)g_heap;
      g_heap += size + META_SIZE;
      best_block->size = size;
      best_block->next = g_lastBlock;
      g_lastBlock = best_block;
   }
   return best_block;
}

static void* _malloc(size_t size) {
   if (g_heap == HEAP_NOT_INIT) { _init_heap(); }
   assert(g_heap != HEAP_NOT_INIT);

   _BlockMeta* block = _find_best_block(size);
   if (block == NULL) {
      errno = ENOMEM;
      return NULL;
   }
   block->free = 0;
   return block+1;
}
static void _free(void* block) {
   _BlockMeta* meta = ((_BlockMeta*)block)-1;
   meta->free = 1;
}

/* API */
__API void* memalloc(size_t BlockSize) {
   return _malloc(BlockSize);
}
__API void* arralloc(size_t ElSize, size_t ElCount) {
   void* block = _malloc(ElSize*ElCount);
   if (block == NULL) { return NULL; }
   ClearMemory(block, ElSize*ElCount);
   return block;
}
__API void* rememalloc(void* OldBlock, size_t NewBlockSize) {
   void* block = _malloc(NewBlockSize);
   if (OldBlock == NULL) { return block; }
   if (block == NULL) { return NULL; }
   memcpy(block, OldBlock, NewBlockSize);
   return block;
}
__API void free(void* Block) {
   _free(Block);
}