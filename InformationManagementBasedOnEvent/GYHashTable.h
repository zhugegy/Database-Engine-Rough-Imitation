#pragma once

#include "GYLinkList.h"
#include <math.h>

template <typename T, int nConstMaxIndex = 10000, int nConstHashSeed = 12345>
class CGYHashTable
{
public:
  CGYHashTable();
  virtual ~CGYHashTable();
private:
  CGYLinkList<T> m_HashTable[nConstMaxIndex];
public:
  int add_string_into_hash_table(const char* pszObj, T tData);
  CGYLinkList<T> * query_string_get_a_link_list(const char * pStr);
private:
  int hash_function_get_index(const char * pStr);
};


template <typename T, int nConstMaxIndex, int nConstHashSeed>
CGYHashTable<T, nConstMaxIndex, nConstHashSeed>::CGYHashTable()
{
}


template <typename T, int nConstMaxIndex, int nConstHashSeed>
CGYHashTable<T, nConstMaxIndex, nConstHashSeed>::~CGYHashTable()
{
}


template <typename T, int nConstMaxIndex, int nConstHashSeed>
int CGYHashTable<T, nConstMaxIndex, nConstHashSeed>::add_string_into_hash_table(const char* pStr, T tData)
{
  int nIndex = hash_function_get_index(pStr);

  (m_HashTable[nIndex]).insert_to_tail(tData);

  return 0;
}


template <typename T, int nConstMaxIndex, int nConstHashSeed>
typename CGYLinkList<T> * CGYHashTable<T, nConstMaxIndex, nConstHashSeed>::query_string_get_a_link_list(const char * pStr)
{
  int nIndex = hash_function_get_index(pStr);

  return &(m_HashTable[nIndex]);
}


template <typename T, int nConstMaxIndex, int nConstHashSeed>
int CGYHashTable<T, nConstMaxIndex, nConstHashSeed>::hash_function_get_index(const char * pStr)
{
  int h = nConstHashSeed;

  for (int i = 0; pStr[i] != 0; ++i)
  {
    h = (int)h ^ ((h << 5) + (h >> 2) + pStr[i]);
  }

  return abs(h % nConstMaxIndex);
}