#include <stdio.h>
#include <string.h>
#include <windows.h>

#include <mem.h>

typedef unsigned short bool_t;

#define TAB__ "\t"

typedef struct _Foo {
   long long Int64;
   long Int32;
   int Int;
   short Int16;
   char Int8;
} Foo;

typedef long Pixel;

bool_t test_primitive() {
   /* 
      Memory Allocator test with primitives(int, float, etc.)
   */

   int* IntegerValue = memalloc(sizeof(int));
   if (!IntegerValue) { return FALSE; }

   printf(TAB__ "Integer allocated at %p, garbage value = %i, ", IntegerValue, *IntegerValue);
   *IntegerValue = 42;
   if (*IntegerValue != 42) { return FALSE; }
   printf("value = %i\n", *IntegerValue);

   free(memalloc(sizeof(long long)));

   /* Trying to reallocate the same type, expected both values will be at the same address */
   int* SavedAddress = IntegerValue;
   free(IntegerValue);
   IntegerValue = memalloc(sizeof(int));
   if (SavedAddress != IntegerValue) {
      printf(TAB__ "Reallocate test failed, expected second int at %p, received at %p\n", SavedAddress, IntegerValue);
      return FALSE;
   }
   printf(TAB__ "Integer allocating with using pre-allocating space test passed. Value = %i\n", *IntegerValue);

   /* Let's allocate something else */
   long long* Int64Value = memalloc(sizeof(long long));
   *Int64Value = 123456789123456789;
   printf(TAB__ "64 bit Integer allocated at %p, value = %lli\n", Int64Value, *Int64Value);

   /* Okey, test passed, need to free memory */
   free(IntegerValue);
   free(Int64Value);

   return TRUE;
}
bool_t test_structure() {
   /* 
      Memory Allocator test with structures
   */

  /* Just allocate Foo structure */
   Foo* foo = memalloc(sizeof(Foo));
   if (!foo) { return FALSE; }
   printf(TAB__ "Foo allocated at %p\n", foo);
   memset(foo, 0, sizeof(Foo));
   foo->Int8 = 128;
   foo->Int16 = 4242;
   foo->Int32 = 102410241024;
   foo->Int64 = 123456789123456789;
   foo->Int = 42;
   free(foo);

   return TRUE;
}
bool_t test_bigdata() {
   /* 
      Memory Allocator test with Big data arrays
   */

   /* For example, we need to allocate space for HD screen with 32 bits for 1 pixel(RGBA) */
   Pixel* screen = memalloc(1920*1080*sizeof(Pixel));
   if (!screen) {
      printf(TAB__ "Can't allocate screen with %i pixels(%ibytes)\n", 1920*1080, 1920*1080*sizeof(Pixel));
      return FALSE;
   }
   printf(TAB__ "Screen with %i(%ibytes) pixels successfuly allocated\n", 1920*1080, 1920*1080*sizeof(Pixel));
   free(screen);

   return TRUE;
}

int main() {
   bool_t success;

   success = test_primitive();
   if (!success) {
      fprintf(stderr, "Primitives test failed\n");
      fflush(stderr);
      return 1;
   }
   fprintf(stdout, "Primitives test passed\n");

   success = test_structure();
   if (!success) {
      fprintf(stderr, "Structures test failed\n");
      fflush(stderr);
      return 1;
   }
   fprintf(stdout, "Structures test passed\n");

   success = test_bigdata();
   if (!success) {
      fprintf(stderr, "Big data test failed\n");
      fflush(stderr);
      return 1;
   }
   fprintf(stdout, "Big data test passed\n");

   fprintf(stdout, "All tests passed\n");
   return 0;
}