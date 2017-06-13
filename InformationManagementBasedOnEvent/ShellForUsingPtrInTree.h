#pragma once

#include "GeneralizedObject.h"

//��Ϊ����tree�Ľڵ�����洢ָ�룬�����Ƕ���ı��£�����Ҫ���������ǣ���ָ���������
//����ĸ�Դ�����ڲ������ڵ��ʱ����Ҫ�Ƚϴ�С����ָ�루�������ͣ��޷��������������
//����������Ŀ�ģ�����Ϊ�˽�����������
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