// The Heap project
// Hashtable(Associative array)
// Free to use and distribute
/// Entry point

#include <thread>
#include <iostream>
#include "HashTable.hpp"

using namespace std;

typedef const char* cstr_t;

int main()
{
   HashTable<cstr_t> table;

   table.Set("SomeName", "Hello, World!");
   table.Set("C++ Creator", "GAY");

   cout << table["SomeName"] << endl;
   cout << "C++ creator is " << table["C++ Creator"] << endl;
   return 0;
}