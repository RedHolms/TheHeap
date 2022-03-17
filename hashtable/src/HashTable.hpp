// The Heap project
// Hashtable(Associative array)
// Free to use and distribute
/// Main Header

#pragma once

#include <exception>
#include <cstring>
#include <cstddef>
#include "hash.h"

class ValueNotExistException : public std::exception
{
public:
   ValueNotExistException(const char* key) : std::exception(), m_key(key) {}
   const char* GetKey() { return m_key; }
   const char* what() { return "Value doesn't exist"; }
private:
   const char* m_key;
};
class TableOverflowException : public std::exception
{
public:
   const char* what() { return "Table overflow"; }
};
class TableCollisionException : public std::exception
{
public:
   TableCollisionException(const char* key, unsigned long idx) : std::exception(), m_key(key), m_idx(idx) {}
   const char* GetKey() { return m_key; }
   unsigned long GetIDX() { return m_idx; }
   const char* what() { return "Key collision"; }
private:
   const char* m_key;
   unsigned long m_idx;
};

template <typename T>
struct HashItem
{
   const char* key;
   T value;
};

template <typename T>
class HashTable
{
public:
   HashTable(size_t reserved = 1000, size_t max = 50000);
   HashTable(hashfn_t hashfn, size_t reserved = 1000, size_t max = 50000);

private:
   size_t m_max;
   size_t m_reserved;
   size_t m_count = 0;
   size_t m_maxIndex = 0;
   hashfn_t m_hashfn = nullptr;
   struct HashItem<T>** m_items = nullptr;

public:
   void  Set(const char* key, T value);
   T&    Get(const char* key);
   void  Delete(const char* key);
   bool  Exist(const char* key);

   T& operator[](const char* key) { return Get(key); }
private:
   void  _realloc_table();
   void  _delete_item(struct HashItem<T>* item);
   struct HashItem<T>* _alloc_item(const char* key, T& value);
};

#include "HashTable.tpp"