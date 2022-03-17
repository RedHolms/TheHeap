// The Heap project
// Hashtable(Associative array)
// Free to use and distribute
/// Hash Function

#ifndef f_hash_h_
#define f_hash_h_

#if defined(__cplusplus)
#define __C_BEGIN extern "C" {
#define __C_END   }
#else
#define __C_BEGIN
#define __C_END
#endif

__C_BEGIN

#include <stddef.h>

typedef unsigned long (*hashfn_t)(const unsigned char*, size_t);
unsigned long _default_hash(const unsigned char* str, size_t limit);

__C_END

#endif // f_hash_h_