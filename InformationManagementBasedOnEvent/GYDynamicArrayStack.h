#pragma once

#include "GYDynamicArray.h"

template<typename T>
class CGYDynamicArrayStack
{
public:
  CGYDynamicArrayStack();
  virtual ~CGYDynamicArrayStack();

public:
  //int push(T tData);
  int push(const T &tData);
  const T & peek();
  bool is_empty();
  bool pop(T & tData);
  T pop();

private:
  CGYDynamicArray<T> m_dyary;
public:
  int __test_print();
};

template<typename T>
CGYDynamicArrayStack<T>::CGYDynamicArrayStack()
{
}


template<typename T>
CGYDynamicArrayStack<T>::~CGYDynamicArrayStack()
{
}


/*
template<typename T>
int CGYDynamicArrayStack<T>::push(T tData)
{
  m_dyary.insert_element(tData, -1);
  return 0;
}*/

template<typename T>
int CGYDynamicArrayStack<T>::push(const T &tData)
{
  m_dyary.insert_element(tData, -1);
  return 0;
}


template<typename T>
const T & CGYDynamicArrayStack<T>::peek()
{
  // TODO: insert return statement here
  int nCount = m_dyary.get_array_element_count();

  return m_dyary[nCount];
}


template<typename T>
bool CGYDynamicArrayStack<T>::is_empty()
{
  if (m_dyary.get_array_element_count() == 0)
  {
    return true;
  }

  return false;
}


template<typename T>
bool CGYDynamicArrayStack<T>::pop(T & tData)
{
  // TODO: insert return statement here
  int nCount = m_dyary.get_array_element_count();

  tData = m_dyary[nCount];

  m_dyary.delete_element(nCount);

  return true;
}


template<typename T>
T CGYDynamicArrayStack<T>::pop()
{
  int nCount = m_dyary.get_array_element_count();

  T tDataTmp = m_dyary[nCount];

  m_dyary.delete_element(nCount);

  return tDataTmp;
}

template<typename T>
int CGYDynamicArrayStack<T>::__test_print()
{
  m_dyary.__test_iterate_elements();

  return 0;
}