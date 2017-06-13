#pragma once

template<typename T>
class CGYLinkList
{
private:
  struct CGYLinkListNode;

public:
  CGYLinkList();
  virtual ~CGYLinkList();

  int get_node_count();
  bool is_empty();
  bool remove_head_node();
  bool remove_tail_node();
  bool __test_print_list();
  bool clear_link_list();

  class iterator;
  iterator insert(const T &tData, const iterator &it);
  iterator insert_to_head(const T &tData);
  iterator insert_to_tail(const T &tData);
  bool get_data_via_iterator(const T &tData, const iterator &it);
  const T & get_data_via_iterator(const iterator &it);
  bool set_data_via_iterator(const T &tData, const iterator &it);
  bool does_node_exist_via_iterator(const iterator &it);
  iterator find_iterator_via_data(const T &tData);
  iterator begin();
  iterator end();
  iterator get_iterator_of_head_node();
  iterator get_iterator_of_tail_node();
  bool remove_via_iterator(const iterator &it);

/*
  for (CMyList<float>::iterator it = lst.begin(); it != lst.end(); it++)
  {
    printf("%f ", (*it));
  }*/

  /*iterator的作用，不想让用户直接获得m_pHeadNode（或者m_pTailNode)，因为一旦用户获
  得这个头尾指针，除了读写指针所指向的节点的数据，还可以自由地修改指针指向的节点pPre和
  pNext，所以进行一个封装(iterator其实就有唯一的一个数据成员CGYLinkListNode *m_pNode
  )。用户只能通过iterator类的对象，来告诉程序自己想去哪个节点，用户只能读写节点内部的数
  据，无法修改节点内部的pPre和pNext。

  iterator的另一个好处，就是让遍历整个链表这个操作变得更加可读、更加方便。*/
  class iterator
  {
  public:
    explicit iterator(CGYLinkListNode *pNode);
    //iterator();
    //~iterator();
  public:
    friend class CGYLinkList;    //写好了试试不加友元声明，好像并没有影响，有影响
    bool operator==(const iterator &obj);
    bool operator!=(const iterator &obj);    
    iterator & operator++();    //前置++    
    iterator & /*&*/ operator++(int);    //后置++    //不太会写
    iterator & operator--();    //前置--    
    iterator & /*&*/ operator--(int);    //后置--    //不太会写
    T& operator*();

  private:
    CGYLinkListNode *m_pNode;
  };

private:
  struct CGYLinkListNode
  {
    CGYLinkListNode()
    {
      pPre = NULL;
      pNext = NULL;
    }

    CGYLinkListNode *pPre;
    CGYLinkListNode *pNext;
    T tData;
  };

  CGYLinkListNode *m_pHeadNode;
  CGYLinkListNode *m_pTailNode;
  int m_nNodeCount;
};

////////////////////////////////////////////////////////////////////////////////
template<typename T>
CGYLinkList<T>::iterator::iterator(CGYLinkListNode *pNode)
{
  m_pNode = pNode;
}

template<typename T>
bool CGYLinkList<T>::iterator::operator==(const iterator &obj)
{
  return m_pNode == obj.m_pNode ? true : false;
}

template<typename T>
bool CGYLinkList<T>::iterator::operator!=(const iterator &obj)
{
  return m_pNode != obj.m_pNode ? true : false;
}

template<typename T>
typename CGYLinkList<T>::iterator& CGYLinkList<T>::iterator::operator++()
{
  m_pNode = m_pNode->pNext;
  return *this;
}

template<typename T>
typename CGYLinkList<T>::iterator& CGYLinkList<T>::iterator::operator++(int)
{
  m_pNode = m_pNode->pNext;
  return *this;
}

template<typename T>
typename CGYLinkList<T>::iterator& CGYLinkList<T>::iterator::operator--()
{
  m_pNode = m_pNode->pPre;
  return *this;
}

template<typename T>
typename CGYLinkList<T>::iterator& CGYLinkList<T>::iterator::operator--(int)
{
  m_pNode = m_pNode->pPre;
  return *this;
}

template<typename T>
T& CGYLinkList<T>::iterator::operator*()
{
  return m_pNode->tData;
}

////////////////////////////////////////////////////////////////////////////////
template<typename T>
typename CGYLinkList<T>::iterator CGYLinkList<T>::begin()
{
  return iterator(m_pHeadNode);
}

template<typename T>
typename CGYLinkList<T>::iterator CGYLinkList<T>::end()
{
  return iterator(NULL);
}

template<typename T>
typename CGYLinkList<T>::iterator CGYLinkList<T>::get_iterator_of_head_node()
{
  return iterator(m_pHeadNode);
}

template<typename T>
typename CGYLinkList<T>::iterator CGYLinkList<T>::get_iterator_of_tail_node()
{
  return iterator(m_pTailNode);
}

template<typename T>
typename CGYLinkList<T>::iterator CGYLinkList<T>::find_iterator_via_data(
  const T &tData)
{
  CGYLinkListNode *pNext = m_pHeadNode;

  while (pNext != NULL)
  {
    if (pNext->tData == tData)
    {
      return iterator(pNext);
    }

    pNext = pNext->pNext;
  }

  return iterator(NULL);    //把空指针NULL作为参数，传入iterator类的构造函数
}

////////////////////////////////////////////////////////////////////////////////

template<typename T>
CGYLinkList<T>::CGYLinkList()
{
  m_pHeadNode = NULL;
  m_pTailNode = NULL;
  m_nNodeCount = 0;
}


template<typename T>
CGYLinkList<T>::~CGYLinkList()
{
  //释放所有Node
  clear_link_list();
}

template<typename T>
int CGYLinkList<T>::get_node_count()
{
  return m_nNodeCount;
}


template<typename T>
bool CGYLinkList<T>::is_empty()
{
  return m_nNodeCount == 0 ? true : false;
}

template<typename T>
bool CGYLinkList<T>::clear_link_list()
{
  CGYLinkListNode *pNext = m_pHeadNode;
  CGYLinkListNode *pTmp = NULL;

  while (pNext != NULL)
  {
    pTmp = pNext;
    pNext = pNext->pNext;

    delete pTmp;
  }

  m_pHeadNode = NULL;
  m_pTailNode = NULL;
  m_nNodeCount = 0;

  return true;
}

template<typename T>
bool CGYLinkList<T>::__test_print_list()
{
  CGYLinkListNode *pNext = m_pHeadNode;

  while (pNext != NULL)
  {
    printf("%d ", pNext->tData);
    pNext = pNext->pNext;
  }
  return false;
}

template<typename T>
bool CGYLinkList<T>::remove_head_node()
{
  if (m_nNodeCount == 0)
  {
    return false;
  }

  if (m_nNodeCount == 1)
  {
    delete m_pHeadNode;
    m_pHeadNode = NULL;
    m_pTailNode = NULL;
    m_nNodeCount = 0;

    return true;
  }

  CGYLinkListNode *pTmp = m_pHeadNode->pNext;
  pTmp->pPre = NULL;
  delete m_pHeadNode;
  m_pHeadNode = pTmp;
  m_nNodeCount--;

  return true;
}

template<typename T>
bool CGYLinkList<T>::does_node_exist_via_iterator(const iterator &it)
{
  CGYLinkListNode *pTmp = it.m_pNode;

  CGYLinkListNode *pNext = m_pHeadNode;

  while (pNext != NULL)
  {
    if (pNext == pTmp)
    {
      return true;
    }

    pNext = pNext->pNext;
  }

  return false;
}

template<typename T>
bool CGYLinkList<T>::remove_tail_node()
{
  if (m_nNodeCount == 0)
  {
    return false;
  }

  if (m_nNodeCount == 1)
  {
    delete m_pTailNode;
    m_pHeadNode = NULL;
    m_pTailNode = NULL;
    m_nNodeCount = 0;

    return true;
  }

  CGYLinkListNode *pTmp = m_pTailNode->pPre;
  pTmp->pNext = NULL;
  delete m_pTailNode;
  m_pTailNode = pTmp;
  m_nNodeCount--;

  return true;
}

template<typename T>
bool CGYLinkList<T>::remove_via_iterator(const iterator &it)
{
  CGYLinkListNode *pTmp = it.m_pNode;
  CGYLinkListNode *pPre = NULL;
  CGYLinkListNode *pNext = NULL;

  if (m_nNodeCount == 0)
  {
    return false;
  }

  if (pTmp == NULL)
  {
    /*传进来就是NULL，就不用删除了（相当于删除成功）？待考虑（用户有可能无差别
    （massively）批量删除，返回false可能会不必要地中断用户的这种行为）。*/
    return false;
  }

  if (does_node_exist_via_iterator(it) == false)
  {
    return false;
  }

  if (m_nNodeCount == 1)
  {
    delete m_pHeadNode;
    m_pHeadNode = NULL;
    m_pTailNode = NULL;
    m_nNodeCount = 0;

    return true;
  }

  pPre = pTmp->pPre;
  pNext = pTmp->pNext;

  if (pPre == NULL)
  {
    m_pHeadNode = pNext;
    pNext->pPre = NULL;
  }
  else if (pNext == NULL)
  {
    m_pTailNode = pPre;
    pPre->pNext = NULL;
  }
  else
  {
    pPre->pNext = pNext;
    pNext->pPre = pPre;
  }

  delete pTmp;
  m_nNodeCount--;

  return true;
}

//向这个节点的尾部插入
template<typename T>
typename CGYLinkList<T>::iterator CGYLinkList<T>::insert(const T &tData, 
  const iterator &it)
{
  CGYLinkListNode *pNode = it.m_pNode;

  if (pNode == NULL)
  {
    return NULL;
  }

  if (does_node_exist_via_iterator(it) == false)
  {
    return NULL;
  }

  CGYLinkListNode *pNewNode = new CGYLinkListNode;
  pNewNode->tData = tData;

  /*判断是否为第一个node，目前来看20170509，函数不可能进入此if下面的语句，因为所有目前
  获得iterator it的方式，都不可能会在空链表里获得一个it*/
  if (m_nNodeCount == 0)
  {
    pNewNode->pPre = NULL;
    pNewNode->pNext = NULL;

    m_pHeadNode = pNewNode;
    m_pTailNode = pNewNode;

    m_nNodeCount++;

    return pNewNode;
  }

  pNewNode->pPre = pNode;
  pNode->pNext = pNewNode;

  pNewNode->pNext = pNode->pNext;
  if (pNode->pNext != NULL)
  {
    pNode->pNext->pPre = pNewNode;
  }
  else
  {
    m_pTailNode = pNewNode;
  }

  m_nNodeCount++;

  return pNewNode;  //直接调用iterator类的构造函数，参数为pNewNode
}

template<typename T>
typename CGYLinkList<T>::iterator CGYLinkList<T>::insert_to_head(const T &tData)
{
  CGYLinkListNode *pNewNode = new CGYLinkListNode;
  pNewNode->tData = tData;

  if (m_nNodeCount == 0)
  {
    pNewNode->pPre = NULL;
    pNewNode->pNext = NULL;

    m_pHeadNode = pNewNode;
    m_pTailNode = pNewNode;

    m_nNodeCount++;

    return iterator(pNewNode);
  }

  pNewNode->pPre = NULL;

  pNewNode->pNext = m_pHeadNode;
  m_pHeadNode->pPre = pNewNode;

  m_pHeadNode = pNewNode;

  m_nNodeCount++;

  return iterator(pNewNode);
}


template<typename T>
typename CGYLinkList<T>::iterator CGYLinkList<T>::insert_to_tail(const T &tData)
{
  CGYLinkListNode *pNewNode = new CGYLinkListNode;
  pNewNode->tData = tData;

  if (m_nNodeCount == 0)
  {
    pNewNode->pPre = NULL;
    pNewNode->pNext = NULL;

    m_pHeadNode = pNewNode;
    m_pTailNode = pNewNode;

    m_nNodeCount++;

    return iterator(pNewNode);
  }

  pNewNode->pPre = m_pTailNode;
  m_pTailNode->pNext = pNewNode;

  pNewNode->pNext = NULL;

  m_pTailNode = pNewNode;

  m_nNodeCount++;

  return iterator(pNewNode);
}

template<typename T>
bool CGYLinkList<T>::set_data_via_iterator(const T &tData, const iterator &it)
{
  CGYLinkListNode *pTmp = it.m_pNode;

  if (pTmp == NULL)
  {
    return false;
  }

  if (does_node_exist_via_iterator(it) == false)
  {
    return false;
  }

  pTmp->tData = tData;

  return true;
}

template<typename T>
bool CGYLinkList<T>::get_data_via_iterator(const T &tData, const iterator &it)
{
  // TODO: insert return statement here
  CGYLinkListNode *pTmp = it.m_pNode;

  if (pTmp == NULL)
  {
    return false;
  }

  //指向非链表node位置的情况
  if (does_node_exist_via_iterator(it) == false)
  {
    return false;
  }

  tData = pTmp->tData;

  return true;
}

//此函数比较危险，存在指向非链表node位置的情况，则会返回无意义随机值
template<typename T>
const T & CGYLinkList<T>::get_data_via_iterator(const iterator &it)
{
  // TODO: insert return statement here
  CGYLinkListNode *pTmp = it.m_pNode;

  return pTmp->tData;
}