// The Heap project
// Hashtable(Associative array)
// Free to use and distribute
/// Hash Function

#include "hash.h"

unsigned long _default_hash(const unsigned char* str, size_t limit) {
   unsigned long hash = 5381;
   unsigned char c;
   while (c = *str++)
      hash = ((hash << 5) + hash) + c;
   return hash % limit;
}