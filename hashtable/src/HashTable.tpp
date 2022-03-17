// The Heap project
// Hashtable(Associative array)
// Free to use and distribute
/// Main Source

#include <cstring>
#include "HashTable.hpp"
#include "hash.h"

template <typename T>
HashTable<T>::HashTable(size_t reserved, size_t max)
{
   m_reserved = reserved;
   m_max = max;
   m_hashfn = _default_hash;
   _realloc_table();
}
template <typename T>
HashTable<T>::HashTable(hashfn_t hashfn, size_t reserved, size_t max)
{
   m_reserved = reserved;
   m_max = max;
   m_hashfn = hashfn;
   _realloc_table();
}

template <typename T>
void HashTable<T>::Set(const char* key, T value)
{
   unsigned long idx = m_hashfn((const unsigned char*)key, m_max);
   if (idx > m_reserved) {
      m_reserved = idx * 2;
      _realloc_table();
   }
   if (m_items[idx])
   {
      if (strcmp(m_items[idx]->key, key) != 0) throw TableCollisionException(key, idx);
      m_items[idx]->value = value;
      return;
   }
   if (m_maxIndex < idx) m_maxIndex = idx;
   m_items[idx] = _alloc_item(key, value);
}

template <typename T>
T& HashTable<T>::Get(const char* key)
{
   if (!Exist(key)) throw ValueNotExistException(key);
   unsigned long idx = m_hashfn((const unsigned char*)key, m_max);
   struct HashItem<T>* item = m_items[idx];
   return item->value;
}

template <typename T>
void HashTable<T>::Delete(const char* key)
{
   if (!Exist(key)) throw ValueNotExistException(key);
   unsigned long idx = m_hashfn((const unsigned char*)key, m_max);
   _delete_item(m_items[idx]);
   m_items[idx] = nullptr;
}

template <typename T>
bool HashTable<T>::Exist(const char* key)
{
   unsigned long idx = m_hashfn((const unsigned char*)key, m_max);
   if (m_maxIndex >= idx)
      if (m_items[idx])
         if (strcmp(m_items[idx]->key, key) != 0)
            throw TableCollisionException(key, idx);
         else return true;
   return false;
}

template <typename T>
struct HashItem<T>* HashTable<T>::_alloc_item(const char* key, T& value)
{
   struct HashItem<T>* item = new struct HashItem<T>;
   item->key = key;
   item->value = value;
   return item;
}

template <typename T>
void HashTable<T>::_realloc_table()
{
   if (m_reserved == 0)
   {
      if (m_items)
      {
         for (size_t i = 0; i <= m_maxIndex; i++)
            if (m_items[i]) _delete_item(m_items[i]);
         delete[] m_items;
      }
      m_items = nullptr;
      m_count = 0;
      m_maxIndex = 0;
      return;
   }

   if (m_reserved >= m_max) throw TableOverflowException();

   struct HashItem<T>** new_items = new struct HashItem<T>*[m_reserved];
   memset(new_items, 0, m_reserved * sizeof(struct HashItem<T>*));
   if (m_items)
   {
      memcpy(new_items, m_items, m_maxIndex * sizeof(struct HashItem<T>*));
      delete[] m_items;
   }
   m_items = new_items;
}

template <typename T>
void HashTable<T>::_delete_item(struct HashItem<T>* item)
{
   delete[] item->key;
   delete[] item->value;
   delete item;
}