#pragma once

#include <stdlib.h>
#include <iostream>
#include "GYDynamicArrayQueue.h"
#include "GYDynamicArrayStack.h"

//平衡二叉排序树
template<typename T>
class CGYAVLTree
{
//private:
public:
  struct CGYAVLTreeNode;

public:
#define max(a,b) (((a) > (b)) ? (a) : (b))

public:
  CGYAVLTree();
  virtual ~CGYAVLTree();

public:
  //增
  bool insert_node(T tData);
  bool insert_node(CGYAVLTreeNode* pNode);

  //删  有问题，暂时先不开放
  // bool delete_node(T data);
  // bool delete_node(CMyTreeNode* pNode);

  //改  就是先删后增
  //UpdateNode();

  //查
  CGYAVLTreeNode* find_node(T tData);

  //遍历树
  // 层序遍历
  void layer_traversal();

  //前根遍历
  void DLR_traversal(CGYAVLTreeNode* pNode);
  //void _DLR_traversal(CMyTreeNode* pNode);

  //中根遍历
  void LDR_traversal(CGYAVLTreeNode* pNode, CGYDynamicArrayQueue<T> & daryStorage);
  //void _LDR_traversal(CMyTreeNode* pNode);

  //后根遍历
  void LRD_traversal(CGYAVLTreeNode* pNode);
  //void _LRD_traversal(CMyTreeNode* pNode);

  //旋转
  void right_rotate(CGYAVLTreeNode* pK1, CGYAVLTreeNode* pK2);
  void left_rotate(CGYAVLTreeNode* pK1, CGYAVLTreeNode* pK2);

  //检查
  bool check_rotation_and_adjust_height(CGYAVLTreeNode* pNode);

  CGYAVLTreeNode* get_root_node();
  int get_height(CGYAVLTreeNode* pNode);

private:
  struct CGYAVLTreeNode
  {
    CGYAVLTreeNode()
    {
      m_pLeft = NULL;
      m_pRight = NULL;
      m_pParent = NULL;
      m_nHeight = 0;
    }

    CGYAVLTreeNode(T tData)
    {
      m_pLeft = NULL;
      m_pRight = NULL;
      m_pParent = NULL;
      m_nHeight = 0;

      m_data = tData;
    }

    CGYAVLTreeNode* m_pLeft;
    CGYAVLTreeNode* m_pRight;
    CGYAVLTreeNode* m_pParent;
    T          m_data;
    int          m_nHeight;//高度
  };

  CGYAVLTreeNode* m_pRoot; //根结点
  int          m_nCount; //结点个数
};

template<typename T>
CGYAVLTree<T>::CGYAVLTree()
{
  m_pRoot = NULL;
  m_nCount = 0;
}

template<typename T>
CGYAVLTree<T>::~CGYAVLTree()
{

}

template<typename T>
typename CGYAVLTree<T>::CGYAVLTreeNode* CGYAVLTree<T>::get_root_node()
{
  return m_pRoot;
}

template<typename T>
int CGYAVLTree<T>::get_height(CGYAVLTreeNode* pNode)
{
  return pNode == NULL ? 0 : pNode->m_nHeight;
}

template<typename T>
bool CGYAVLTree<T>::insert_node(T tData)
{
  CGYAVLTreeNode* pNode = new CGYAVLTreeNode(tData);

  return insert_node(pNode);
}

template<typename T>
bool CGYAVLTree<T>::insert_node(CGYAVLTreeNode* pNode)
{
  if (pNode == NULL)
  {
    return false;
  }

  CGYAVLTreeNode* pCur = m_pRoot;

  if (pCur == NULL)
  {
    m_pRoot = pNode;
    m_nCount++;
    return true;
  }

  while (true)
  {
    if (pCur->m_data > pNode->m_data)
    {
      if (pCur->m_pLeft != NULL)
      {
        pCur = pCur->m_pLeft;
        continue;
      }
      else
      {
        pCur->m_pLeft = pNode;
      }

    }
    else if (pCur->m_data < pNode->m_data)
    {
      if (pCur->m_pRight != NULL)
      {
        pCur = pCur->m_pRight;
        continue;
      }
      else
      {
        pCur->m_pRight = pNode;
      }
    }
    else
    {
      //((CShellForUsingPtrInTree<CGeneralizedObject>)(pCur->m_data)).set_content_ptr(NULL);
      if (pCur->m_pParent != NULL)
      {
        if (pCur->m_pParent->m_pLeft == pCur)
        {
          pCur->m_pParent->m_pLeft = pNode;
        }
        else
        {
          pCur->m_pParent->m_pRight = pNode;
        }
      }

      if (pCur->m_pLeft != NULL)
      {
        pCur->m_pLeft->m_pParent = pNode;
      }

      if (pCur->m_pRight != NULL)
      {
        pCur->m_pRight->m_pParent = pNode;
      }


      pNode->m_pParent = pCur->m_pParent;
      pNode->m_pLeft = pCur->m_pLeft;
      pNode->m_pRight = pCur->m_pRight;
      pNode->m_nHeight = pCur->m_nHeight;

      /* CGYAVLTreeNode* m_pLeft;
          CGYAVLTreeNode* m_pRight;
          CGYAVLTreeNode* m_pParent;
          T          m_data;
          int          m_nHeight;//高度*/

      delete pCur;
      return true;
    }

    pNode->m_pParent = pCur;
    m_nCount++;

    check_rotation_and_adjust_height(pNode);

    return true;
  }

  return false;
}

template<typename T>
void CGYAVLTree<T>::DLR_traversal(CGYAVLTreeNode* pNode)
{
  if (pNode == NULL)
  {
    return;
  }

  //printf("%lf ", pNode->m_data);
  std::cout << pNode->m_data << " ";
  DLR_traversal(pNode->m_pLeft);
  DLR_traversal(pNode->m_pRight);
}

template<typename T>
void CGYAVLTree<T>::LDR_traversal(CGYAVLTreeNode* pNode, CGYDynamicArrayQueue<T> & daryStorage)
{
  if (pNode == NULL)
  {
    return;
  }


  LDR_traversal(pNode->m_pLeft, daryStorage);
  //printf("%lf ", pNode->m_data);
  //std::cout << pNode->m_data << " ";
  daryStorage.add(pNode->m_data);
  LDR_traversal(pNode->m_pRight, daryStorage);
}

template<typename T>
void CGYAVLTree<T>::LRD_traversal(CGYAVLTreeNode* pNode)
{
  if (pNode == NULL)
  {
    return;
  }

  LRD_traversal(pNode->m_pLeft);
  LRD_traversal(pNode->m_pRight);
  //printf("%lf ", pNode->m_data);
  std::cout << pNode->m_data << " ";
}

template<typename T>
void CGYAVLTree<T>::layer_traversal()
{
  CGYAVLTreeNode* pNode = m_pRoot;
  if (pNode == NULL)
  {
    return;
  }

  CGYDynamicArrayQueue<CGYAVLTreeNode*> queue;
  queue.add(pNode);

  while (!queue.is_empty())
  {
    CGYAVLTreeNode* pNode = queue.get();
    //printf("%lf ", pNode->m_data);
    std::cout << pNode->m_data << " ";

    if (pNode->m_pLeft != NULL)
    {
      queue.add(pNode->m_pLeft);
    }

    if (pNode->m_pRight != NULL)
    {
      queue.add(pNode->m_pRight);
    }
  }
}

template<typename T>
typename CGYAVLTree<T>::CGYAVLTreeNode* CGYAVLTree<T>::find_node(T tData)
{
  CGYAVLTreeNode* pFind = m_pRoot;

  while (pFind != NULL)
  {
    if (pFind->m_data > tData)
    {
      pFind = pFind->m_pLeft;
      continue;
    }
    else if (pFind->m_data < tData)
    {
      pFind = pFind->m_pRight;
      continue;
    }
    else
    {
      return pFind;
    }
  }

  return NULL;
}


/*
右单旋:
30     -----      20
20       -----    10  30
10         -----


60                      60
40   X  -----         30       X
30  50    -----      20   40
20  35      -----    10    35 50
10            -----

G                   G
K1       -----      K2
K2  C      -----    A    K1
A   B        -----       B    C
*/
template<typename T>
void CGYAVLTree<T>::right_rotate(CGYAVLTreeNode* pK1, CGYAVLTreeNode* pK2)
{
  CGYAVLTreeNode* pG = pK1->m_pParent;
  CGYAVLTreeNode* pA = pK2->m_pLeft;
  CGYAVLTreeNode* pB = pK2->m_pRight;
  CGYAVLTreeNode* pC = pK1->m_pRight;

  //改变父子关系 
  if (pG == NULL)
  {
    m_pRoot = pK2;
  }
  else
  {
    if (pG->m_pLeft == pK1)
    {
      pG->m_pLeft = pK2;
    }
    else if (pG->m_pRight == pK1)
    {
      pG->m_pRight = pK2;
    }
  }

  pK1->m_pLeft = pB;
  pK1->m_pParent = pK2;

  pK2->m_pParent = pG;
  pK2->m_pRight = pK1;


  if (pB != NULL)
  {
    pB->m_pParent = pK1;
  }

  pK1->m_nHeight = max(get_height(pB), get_height(pC)) + 1;
  pK2->m_nHeight = max(get_height(pA), get_height(pK1)) + 1;
}

/*
左单旋:
30         -----      40
40       -----    30  50
50     -----



G                    G
K1        -----      K2
C  K2      -----    K1  B
A   B    -----   C  A
*/
template<typename T>
void CGYAVLTree<T>::left_rotate(CGYAVLTreeNode* pK1, CGYAVLTreeNode* pK2)
{
  CGYAVLTreeNode* pG = pK1->m_pParent;
  CGYAVLTreeNode* pA = pK2->m_pLeft;
  CGYAVLTreeNode* pB = pK2->m_pRight;
  CGYAVLTreeNode* pC = pK1->m_pLeft;

  //改变父子关系
  if (pG == NULL)
  {
    m_pRoot = pK2;
  }
  else
  {
    if (pG->m_pLeft == pK1)
    {
      pG->m_pLeft = pK2;
    }
    else if (pG->m_pRight == pK1)
    {
      pG->m_pRight = pK2;
    }
  }

  pK1->m_pRight = pA;
  pK1->m_pParent = pK2;

  pK2->m_pParent = pG;
  pK2->m_pLeft = pK1;


  if (pA != NULL)
  {
    pA->m_pParent = pK1;
  }

  pK1->m_nHeight = max(get_height(pA), get_height(pC)) + 1;
  pK2->m_nHeight = max(get_height(pB), get_height(pK1)) + 1;
}

template<typename T>
bool CGYAVLTree<T>::check_rotation_and_adjust_height(CGYAVLTreeNode* pNode)
{
  bool bRotationFinished = false;

  while (pNode != NULL)
  {
    int nLeftHeight = get_height(pNode->m_pLeft);
    int nRightHeight = get_height(pNode->m_pRight);

    pNode->m_nHeight = max(nLeftHeight, nRightHeight) + 1;

    if (bRotationFinished == false)
    {
      int nSubHeight = nLeftHeight - nRightHeight;

      CGYAVLTreeNode* pK1 = NULL;
      CGYAVLTreeNode* pK2 = NULL;
      CGYAVLTreeNode* pK3 = NULL;

      //判断高度差是否 >= 2
      if (abs(nSubHeight) >= 2)
      {
        pK1 = pNode;
        if (nSubHeight > 0)
        {
          pK2 = pK1->m_pLeft;
        }
        else
        {
          pK2 = pK1->m_pRight;
        }

        if (get_height(pK2->m_pLeft) - get_height(pK2->m_pRight) > 0)
        {
          pK3 = pK2->m_pLeft;
        }
        else
        {
          pK3 = pK2->m_pRight;
        }

        //四种旋转
        //右单旋
        if (pK2 == pK1->m_pLeft && pK3 == pK2->m_pLeft)
        {
          right_rotate(pK1, pK2);
        }
        //左单旋
        else if (pK2 == pK1->m_pRight && pK3 == pK2->m_pRight)
        {
          left_rotate(pK1, pK2);
        }
        //双旋
        /*
        30          -----        35
        40        -----     30    40
        35          -----
        */
        else if (pK2 == pK1->m_pRight && pK3 == pK2->m_pLeft)
        {
          right_rotate(pK2, pK3);
          left_rotate(pK1, pK3);
        }
        /*
        30          -----        25
        20            -----     20    30
        25          -----
        */
        else if (pK2 == pK1->m_pLeft && pK3 == pK2->m_pRight)
        {
          left_rotate(pK2, pK3);
          right_rotate(pK1, pK3);
        }

        bRotationFinished == true;
      }
    }

    pNode = pNode->m_pParent;
  }

  return true;
}
