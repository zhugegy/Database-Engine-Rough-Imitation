#pragma once

#include "GYDynamicArray.h"

template<typename T>
class CGYDynamicArrayQueue
{
public:
  CGYDynamicArrayQueue();
  virtual ~CGYDynamicArrayQueue();

private:
  CGYDynamicArray<T> m_dyary;


public:
  //bool add(T tData);
  bool add(const T &tData);
  bool get(T &tData);
  T get();
  const T & peek();
  bool is_empty();

public:
  int __test_print();
};


template<typename T>
CGYDynamicArrayQueue<T>::CGYDynamicArrayQueue()
{
}


template<typename T>
CGYDynamicArrayQueue<T>::~CGYDynamicArrayQueue()
{
}

/*
template<typename T>
bool CGYDynamicArrayQueue<T>::add(T tData)
{
  return m_dyary.insert_element(tData, -1);
}*/

template<typename T>
bool CGYDynamicArrayQueue<T>::add(const T &tData)
{
  return m_dyary.insert_element(tData, -1);
}


template<typename T>
bool CGYDynamicArrayQueue<T>::get(T & tData)
{
  if (is_empty() == true)
  {
    return false;
  }

  tData = m_dyary[0];
  m_dyary.delete_element(0);

  return true;
}


template<typename T>
T CGYDynamicArrayQueue<T>::get()
{
/*
  if (is_empty() == true)
  {
    return -1;
  }*/

  T tData = m_dyary[0];

  m_dyary.delete_element(0);

  return tData;
}


template<typename T>
const T & CGYDynamicArrayQueue<T>::peek()
{
  // TODO: insert return statement here
  return m_dyary[0];
}


template<typename T>
bool CGYDynamicArrayQueue<T>::is_empty()
{
  if (m_dyary.get_array_element_count() == 0)
  {
    return true;
  }

  return false;
}

template<typename T>
int CGYDynamicArrayQueue<T>::__test_print()
{
  m_dyary.__test_iterate_elements();

  return 0;
}