#pragma once

#include "GYDynamicArray.h"
#include "GYSingleInstanceMacro.h"
#include "GYString.h"

//单例类
template <typename T>
class CGYObjFactory
{
public:
  SINGLE_INSTANCE(CGYObjFactory<T>)

private:
  CGYObjFactory();
  virtual ~CGYObjFactory();
  CGYDynamicArray<T *> m_aryptData;
public:
  T* get_an_object();

private:
  bool release_all_objects_in_factory();
};

template <typename T>
CGYObjFactory<T>::CGYObjFactory()
{
}


template <typename T>
CGYObjFactory<T>::~CGYObjFactory()
{
  release_all_objects_in_factory();
}


//没有错误检查
template <typename T>
T* CGYObjFactory<T>::get_an_object()
{
  T *pTmp = NULL;

  pTmp = new T;

  m_aryptData.insert_element(pTmp);

  return pTmp;
}

template <typename T>
bool CGYObjFactory<T>::release_all_objects_in_factory()
{
  int nObjectNum = m_aryptData.get_array_element_count();

  for (int i = 0; i < nObjectNum; i++)
  {
    delete m_aryptData[i];
  }

  return true;
}