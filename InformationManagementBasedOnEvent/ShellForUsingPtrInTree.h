#pragma once

#include "GeneralizedObject.h"

//因为想在tree的节点里里存储指针，而不是对象的本事，所以要借助这个外壳，把指针包裹起来
//问题的根源，在于插入树节点的时候，需要比较大小，而指针（基本类型）无法进行运算符重载
//创建这个类的目的，就是为了解决运算符重载
template <typename T>
class CShellForUsingPtrInTree
{
public:
  CShellForUsingPtrInTree();
  CShellForUsingPtrInTree(T *pContentPtr);
  virtual ~CShellForUsingPtrInTree();

public:
  int set_content_ptr(T * pContentPtr);
  bool operator>(const CShellForUsingPtrInTree<T> & obj);
  bool operator<(const CShellForUsingPtrInTree<T> & obj);
  bool operator==(const CShellForUsingPtrInTree<T> & obj);
  bool operator!=(const CShellForUsingPtrInTree<T> & obj);

public:
  T *m_pContentPtr;
};

template <typename T>
CShellForUsingPtrInTree<T>::CShellForUsingPtrInTree()
{
  m_pContentPtr = NULL;
}

template <typename T>
CShellForUsingPtrInTree<T>::CShellForUsingPtrInTree(T *pContentPtr)
{
  m_pContentPtr = pContentPtr;
}

template <typename T>
CShellForUsingPtrInTree<T>::~CShellForUsingPtrInTree()
{
}


template <typename T>
int CShellForUsingPtrInTree<T>::set_content_ptr(T * pContentPtr)
{
  m_pContentPtr = pContentPtr;

  return 0;
}


template <typename T>
bool CShellForUsingPtrInTree<T>::operator>(const CShellForUsingPtrInTree<T> & obj)
{
  return *m_pContentPtr > *(obj.m_pContentPtr);
}

template <typename T>
bool CShellForUsingPtrInTree<T>::operator<(const CShellForUsingPtrInTree<T> & obj)
{
  return *m_pContentPtr < *(obj.m_pContentPtr);
}

template <typename T>
bool CShellForUsingPtrInTree<T>::operator==(const CShellForUsingPtrInTree<T> & obj)
{
  return *m_pContentPtr == *(obj.m_pContentPtr);
}

template <typename T>
bool CShellForUsingPtrInTree<T>::operator!=(const CShellForUsingPtrInTree<T> & obj)
{
  return *m_pContentPtr != *(obj.m_pContentPtr);
}