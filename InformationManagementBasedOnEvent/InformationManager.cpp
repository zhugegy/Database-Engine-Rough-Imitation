#include "stdafx.h"
#include "InformationManager.h"
#include "GYSingleInstanceMacro.h"
#include "GYObjFactory.h"
#include "ShellForUsingPtrInTree.h"
#include "Student.h"
#include "Course.h"
#include "EventFYJC.h"
#include <windows.h>

CInformationManager::CInformationManager()
  :m_strOutputBuffer(INTERFACE_WELCOME_INFO), 
   m_strCommandFormatRestriction(COMMAND_LINE_FORMAT_RESTRICTION)
{
  init();
}


CInformationManager::~CInformationManager()
{
  save_static_serials();
}


int CInformationManager::init()
{
  
  CStudent::init_when_program_startup_student();
  CCourse::init_when_program_startup_course();
  CEventFYJC::init_when_program_startup_eventFYJC();

  //���ļ�����ID������
  //load_static_serials();

  //������ѯר�ö���
  objTemplateForQuery.m_pContentPtr = new CGeneralizedObject(0, NULL);

  //��ʾ��ӭ��Ϣ
  #if defined(_GUI_SDK32)
  // GUI SDK32 programming
  //MessageBox(NULL, (LPCTSTR)*m_strOutputBuffer, TEXT("��ӭ"), NULL);
  #elif defined(_CONSOLE)
  printf(*m_strOutputBuffer);
  #endif
  
  
  return 0;
}


int CInformationManager::add_into_data_storage(eDataStructureName eName, CGeneralizedObject *pObj)
{
  CShellForUsingPtrInTree<CGeneralizedObject> shellObj(pObj);

  switch (eName)
  {
  case CInformationManager::STUDENT_DATA:
    m_treeStudent.insert_node(shellObj);

    //ͬʱ��������������hashtable
    m_hashtableStudentName.add_string_into_hash_table(**(pObj->get_name()), 
      pObj->get_ID());
    break;
  case CInformationManager::COURSE_DATA:
    m_treeCourse.insert_node(shellObj);

    //ͬʱ��������������hashtable
    m_hashtableCourseName.add_string_into_hash_table(**(pObj->get_name()),
      pObj->get_ID());
    break;
  case CInformationManager::EVENTFYJC_DATA:
    m_EventArray.insert_element(pObj, pObj->get_ID());
    break;
  default:
    break;
  }

  return 0;
}





int CInformationManager::main_loop()
{
  while (true)
  {
    //1. ��ȡ�û�ָ��
    get_user_command();

    //2. �����û�ָ��
    if (parser_user_command() == false)
    {
      m_strOutputBuffer = INTERFACE_INPUT_ERROR_INFO;
      printf(*m_strOutputBuffer);
      continue;
    }

    //3. ����ָ��
    if (process_user_command() == false)
    {
      m_strOutputBuffer = INTERFACE_PROCESS_ERROR_INFO;
    }
    printf(*m_strOutputBuffer);


    //����Ū�˳������ˣ�͵����
    if (*(m_daryPstrCommands[0]) == "-q")
    {
      break;
    }
  }
  return 0;
}

//��ȡ�û����룬���������ݳ�Ա m_chStrCommand
int CInformationManager::get_user_command()
{
  scanf(*m_strCommandFormatRestriction, m_chStrCommand);

  while (getchar() != '\n');    //scanf()�����stdin����

  return 0;
}

int CInformationManager::get_user_command(LPTSTR szCmd)
{
  strcpy(m_chStrCommand, szCmd);

  return 0;
}

int CInformationManager::flush_output_buffer()
{
  m_strOutputBuffer = "";
  return  0;
}

LPTSTR CInformationManager::get_output_buffer()
{
  return (LPTSTR)*m_strOutputBuffer;
}

bool CInformationManager::parser_user_command()
{
  char *szTmp = NULL;

  //1. ����Ƿ���ż�������ţ�"��
  int nQuoterCounter = 0;    //���ڼ�����Ÿ����Ƿ�Ϊż��
  szTmp = m_chStrCommand;

  while (*szTmp != '\0')
  {
    //����������ţ�������ǰ�治��ת���
    if (*szTmp == '"' && *(szTmp - 1) != '\\')
    {
      nQuoterCounter++;
    }
    szTmp++;
  }

  if (nQuoterCounter % 2 != 0)
  {
    return false;
  }

  //2. ������ݳ�Ա CGYDynamicArray<CGYString *> m_daryPstrCommands��ӭ����ָ��
  m_daryPstrCommands.empty_array();

  //3. �û�ָ��Ԥ������˫�����м京�еĿո���ʱת��ΪЦ����'\1'��
  int nChangeFlag = 0;    //�жϴ˴��Ƿ�����޸ĵ�flag��
  szTmp = m_chStrCommand;

  while (*szTmp != '\0')
  {
    //����������ţ�������ǰ�治��ת���.
    if (*szTmp == '"' && *(szTmp - 1) != '\\')
    {
      //�ı�flag״̬��
      nChangeFlag = (nChangeFlag == 0) ? 1 : 0;
    }

    if (nChangeFlag == 1 && *szTmp == ' ')
    {
      *szTmp = '\1';
    }

    szTmp++;
  }

  //4. ȥ���û�ָ�����˵Ķ���ո�
  szTmp = m_chStrCommand;

  while (*szTmp == ' ')
  {
    szTmp++;
  }

  char *szTmpTail = szTmp;

  while (*szTmpTail != '\0')
  {
    szTmpTail++;
  }

  szTmpTail--;

  while (*szTmpTail == ' ')
  {
    *szTmpTail = '\0';
    szTmpTail--;
  }

  //5. �����û�ָ��
  char szSubCmd[MAX_COMMAND_LINE_LENGTH] = { 0 };
  int nCounter = 0;
  CGYObjFactory<CGYString> *pStrFactory = GET_SINGLE(CGYObjFactory<CGYString>);
  CGYString *pStrTmp = NULL;

  while (*szTmp != '\0')
  {
    //��ֹ�м���ֶ���ո�
    while (*szTmp == ' ')
    {
      szTmp++;
    }

    szSubCmd[nCounter] = *szTmp;
    nCounter++;
    szTmp++;

    if (*szTmp == ' ')
    { 
      szSubCmd[nCounter] = '\0';
      post_posses_of_char_01(szSubCmd);
      pStrTmp = pStrFactory->get_an_object();
      *pStrTmp = szSubCmd;
      m_daryPstrCommands.insert_element(pStrTmp);
      nCounter = 0;
      szTmp++;
    }

  }

  szSubCmd[nCounter] = '\0';
  post_posses_of_char_01(szSubCmd);
  pStrTmp = pStrFactory->get_an_object();
  *pStrTmp = szSubCmd;
  m_daryPstrCommands.insert_element(pStrTmp);

  //6. ����һ��ָ���Ƿ񳤶�Ϊ2���ҵ�һ���ַ�Ϊ'-'
  if (strlen(**(m_daryPstrCommands[0])) != 2 || 
    *((**(m_daryPstrCommands[0])) + 0) != '-')
  {
    return false;
  }

  return true;
}


int CInformationManager::post_posses_of_char_01(char * pszSubCmd)
{
  char *pchTmp = pszSubCmd;

  while (*pchTmp != '\0')
  {
    if (*pchTmp == '\1')
    {
      *pchTmp = ' ';
    }

    pchTmp++;
  }

  return 0;
}


bool CInformationManager::process_user_command()
{
  m_strOutputBuffer = "";
  char chOperation = *((**(m_daryPstrCommands[0])) + 1);
  bool bIsOperationASuccess = false;

  switch (chOperation)
  {
  case 'a':  //����ѧ��
  case 'A':  //���ӿγ�
    /*bIsOperationASuccess = */cmd_a(chOperation, m_daryPstrCommands[1]);
    break;
  case 'l':  //��ʾѧ��
  case 'L':  //��ʾ�γ�
    bIsOperationASuccess = cmd_l(chOperation);
    break;
  case 'g':  //�����������ѧ��
  case 'G':  //������������γ�
    bIsOperationASuccess = cmd_g(chOperation);
    break;
  case 'd':  //ɾ��ѧ��
  case 'D':  //ɾ���γ�
    bIsOperationASuccess = cmd_d(chOperation);
    break;
  case 'm':  //�޸�ѧ����Ϣ������ + �����ۣ�������ʱ��ע�ͣ���
  case 'M':  //�޸Ŀγ���Ϣ�����ƣ�
    bIsOperationASuccess = cmd_m(chOperation);
    break;
  case 'c':
    bIsOperationASuccess = cmd_c(atoi(**(m_daryPstrCommands[1])),
      atoi(**(m_daryPstrCommands[2])));
    break;
  case 'x':
    bIsOperationASuccess = cmd_x(atoi(**(m_daryPstrCommands[1])),
      atoi(**(m_daryPstrCommands[2])));
    break;
  case 'z':
    bIsOperationASuccess = cmd_z(atoi(**(m_daryPstrCommands[1])),
      atoi(**(m_daryPstrCommands[2])));
    break;
  default:
    break;
  }

  return true;
}

int CInformationManager::cmd_a(char chOperation, CGYString * cmd1)
{
  CGYObjFactory<CGYString> *pStringFactory = GET_SINGLE(
    CGYObjFactory<CGYString>);
  CGYString *pStrTmp = NULL;

  CGeneralizedObject *pGeneralizedObj = NULL;

  switch (chOperation)
  {
  case 'a':
    {
      CGYObjFactory<CStudent> *pStudentFactory = GET_SINGLE(
        CGYObjFactory<CStudent>);
      CStudent *pStuTmp = pStudentFactory->get_an_object();

      add_into_data_storage(CInformationManager::STUDENT_DATA, pStuTmp);
      pGeneralizedObj = pStuTmp;
    }
    break;
  case 'A':
    {
      CGYObjFactory<CCourse> *pCourseFactory = GET_SINGLE(
        CGYObjFactory<CCourse>);
      CCourse *pCourseTmp = pCourseFactory->get_an_object();

      add_into_data_storage(CInformationManager::COURSE_DATA, pCourseTmp);
      pGeneralizedObj = pCourseTmp;
    }
    break;
  case 'E':
    {
    CGYObjFactory<CEventFYJC> *pEventFYJCFactory = GET_SINGLE(
      CGYObjFactory<CEventFYJC>);
    CEventFYJC *pEventFYJCTmp = pEventFYJCFactory->get_an_object();

    add_into_data_storage(CInformationManager::EVENTFYJC_DATA, pEventFYJCTmp);
    pGeneralizedObj = pEventFYJCTmp;
    }
    break;
  default:
    break;
  }

  if (m_daryPstrCommands.get_array_element_count() >= 2 && cmd1 != NULL)
  {
    remove_cmd_quoters(cmd1);
    pStrTmp = pStringFactory->get_an_object();
    pStrTmp->set_string(**cmd1);
    pGeneralizedObj->set_name(pStrTmp);
  }

//   if (m_daryPstrCommands.get_array_element_count() >= 3 && m_daryPstrCommands[2] != NULL)
//   {
//     remove_cmd_quoters(m_daryPstrCommands[2]);
// 
//     pStrTmp = pStringFactory->get_an_object();
//     pStrTmp->set_string(**(m_daryPstrCommands[2]));
//     CStudent *pStuTmp = (CStudent *)pGeneralizedObj;  �ؼ�������
//     pStuTmp->set_comments(pStrTmp);
//   }


  pGeneralizedObj->when_being_added(&m_strOutputBuffer, OUTPUT_BUFFER_LENGTH);
  append_output_buffer(INTERFACE_ADD_SUCCESS_INFO);

  return pGeneralizedObj->get_ID();
}


int CInformationManager::remove_cmd_quoters(CGYString * pStrCmd)
{
  int nLength = (*pStrCmd).get_string_length();

  for (int i = 0; i < nLength - 1; i++)
  {
    (*pStrCmd)[i] = (*pStrCmd)[i + 1];
  }

  (*pStrCmd)[nLength - 2] = '\0';

  return 0;
}


int CInformationManager::append_output_buffer(const char * pStrBuffer)
{
  if (m_strOutputBuffer.get_string_length() < OUTPUT_BUFFER_LENGTH)
  {
    m_strOutputBuffer += pStrBuffer;
  }

  return 0;
}


bool CInformationManager::cmd_l(char chSpecificObj_stu_or_course)
{
  if (m_daryPstrCommands.get_array_element_count() == 1)
  {
    CGYDynamicArrayQueue<CShellForUsingPtrInTree<CGeneralizedObject>> que;

    switch (chSpecificObj_stu_or_course)
    {
    case 'l':
      m_treeStudent.LDR_traversal(m_treeStudent.get_root_node(), que);
      break;
    case 'L':
      m_treeCourse.LDR_traversal(m_treeCourse.get_root_node(), que);
      break;
    default:
      break;
    }

    while (que.is_empty() != true)
    {
      CShellForUsingPtrInTree<CGeneralizedObject> tmp = que.get();
      CGeneralizedObject *pObj = tmp.m_pContentPtr;

      pObj->when_being_listed(&m_strOutputBuffer, OUTPUT_BUFFER_LENGTH);
    }

  }

  if (m_daryPstrCommands.get_array_element_count() == 2 &&
      (*(m_daryPstrCommands[1])).is_consisted_of_pure_number() == true)
  {
    int nId = atoi(**(m_daryPstrCommands[1]));
    cmd_l_num(chSpecificObj_stu_or_course, nId);
  }

  if (m_daryPstrCommands.get_array_element_count() == 2 &&
    (*(m_daryPstrCommands[1])).is_consisted_of_pure_number() == false)
  {
    remove_cmd_quoters(m_daryPstrCommands[1]);
    CGYLinkList<int> *pobjTmp = NULL;
    
    switch (chSpecificObj_stu_or_course)
    {
    case 'l':
      pobjTmp = m_hashtableStudentName.query_string_get_a_link_list(
        **(m_daryPstrCommands[1]));    //=���������
      break;
    case 'L':
      pobjTmp = m_hashtableCourseName.query_string_get_a_link_list(
        **(m_daryPstrCommands[1]));    //=���������
      break;
    default:
      break;
    }
    
    for (CGYLinkList<int>::iterator it = pobjTmp->begin(); it != pobjTmp->end();
      it++)
    {
      CGYString *pTmpName =
        find_name_in_tree_via_ID(chSpecificObj_stu_or_course, *it);

      if (pTmpName != NULL && *(m_daryPstrCommands[1]) == *pTmpName)
      {
        cmd_l_num(chSpecificObj_stu_or_course, *it);
      }
    }
    

  }
  else
  {
    return false;
  }

  append_output_buffer(INTERFACE_LIST_SUCCESS_INFO);

  return true;
}


CGYString * CInformationManager::find_name_in_tree_via_ID(
  char chSpecifyWhichTreeToLookInto, int nID)
{
  (*(objTemplateForQuery.m_pContentPtr)).set_ID(nID);

  CGYAVLTree<CShellForUsingPtrInTree<CGeneralizedObject>>::CGYAVLTreeNode
    *pTreeNode = NULL;

  switch (chSpecifyWhichTreeToLookInto)
  {
  case 'l':
    pTreeNode = m_treeStudent.find_node(objTemplateForQuery);
    break;
  case 'L':
    pTreeNode = m_treeCourse.find_node(objTemplateForQuery);
    break;
  default:
    break;
  }

  if (pTreeNode != NULL)
  {
    CShellForUsingPtrInTree<CGeneralizedObject> tmp = (*pTreeNode).m_data;
    return (tmp.m_pContentPtr)->get_name();
  }

  return NULL;
}


bool CInformationManager::cmd_m(char chOperation)
{
  CGYObjFactory<CGYString> *pStringFactory = GET_SINGLE(
    CGYObjFactory<CGYString>);
  CGYString *pStrTmp = NULL;

  if (m_daryPstrCommands.get_array_element_count() < 3)
  {
    return false;
  }

  int nID = atoi(**(m_daryPstrCommands[1]));

  (*(objTemplateForQuery.m_pContentPtr)).set_ID(nID);

  CGYAVLTree<CShellForUsingPtrInTree<CGeneralizedObject>>::CGYAVLTreeNode
    *pTreeNode = NULL;

  switch (chOperation)
  {
  case 'm':
    pTreeNode = m_treeStudent.find_node(objTemplateForQuery);
    break;
  case 'M':
    pTreeNode = m_treeCourse.find_node(objTemplateForQuery);
    break;
  default:
    break;
  }

  if (pTreeNode == NULL)
  {
    append_output_buffer(INTERFACE_MODIFY_ERROR_INFO);
    return false;
  }
  else
  {
    CShellForUsingPtrInTree<CGeneralizedObject> tmp = (*pTreeNode).m_data;

    CGeneralizedObject *pObj = tmp.m_pContentPtr;

    remove_cmd_quoters(m_daryPstrCommands[2]);
    pStrTmp = pStringFactory->get_an_object();
    pStrTmp->set_string(**(m_daryPstrCommands[2]));
    pObj->set_name(pStrTmp);

    /*ѧ����������Ϣ�޸ģ��ٴκ��ԣ��ؼ�������
    CStudent *pStuTmp = (CStudent *)pGeneralizedObj;��Ҳ���ǰѻ���ת��Ϊ�����࣬Ȼ
    �����set_comments()����*/

    //   if (m_daryPstrCommands.get_array_element_count() >= 3 && m_daryPstrCommands[2] != NULL)
    //   {
    //     remove_cmd_quoters(m_daryPstrCommands[2]);
    // 
    //     pStrTmp = pStringFactory->get_an_object();
    //     pStrTmp->set_string(**(m_daryPstrCommands[2]));
    //     CStudent *pStuTmp = (CStudent *)pGeneralizedObj;
    //     pStuTmp->set_comments(pStrTmp);
    //   }

    pObj->when_being_modified(&m_strOutputBuffer, OUTPUT_BUFFER_LENGTH);
    append_output_buffer(INTERFACE_MODIFY_SUCCESS_INFO);
  }

  return true;
}

bool CInformationManager::cmd_l_num(char chSpecificObj_stu_or_course, int nID)
{
  (*(objTemplateForQuery.m_pContentPtr)).set_ID(nID);

  CGYAVLTree<CShellForUsingPtrInTree<CGeneralizedObject>>::CGYAVLTreeNode
    *pTreeNode = NULL;

  switch (chSpecificObj_stu_or_course)
  {
  case 'l':
    pTreeNode = m_treeStudent.find_node(objTemplateForQuery);
    break;
  case 'L':
    pTreeNode = m_treeCourse.find_node(objTemplateForQuery);
    break;
  default:
    break;
  }

  if (pTreeNode == NULL)
  {
    append_output_buffer(INTERFACE_LIST_ERROR_INFO);
    return false;
  }
  else
  {
    CShellForUsingPtrInTree<CGeneralizedObject> tmp = (*pTreeNode).m_data;

    CGeneralizedObject *pObj = tmp.m_pContentPtr;

    pObj->when_being_listed(&m_strOutputBuffer, OUTPUT_BUFFER_LENGTH);
  }

  return true;
}


bool CInformationManager::cmd_g(char chOperation)
{
  if (m_daryPstrCommands.get_array_element_count() < 2)
  {
    return false;
  }

  int nNumber = atoi(**(m_daryPstrCommands[1]));
  m_daryPstrCommands.set_array_element_count(1);

  for (int i = 0; i < nNumber; i++)
  {
    if (chOperation == 'g')
    {
      cmd_a('a', NULL);
    }
    else
    {
      cmd_a('A', NULL);
    }
  }
  
  return true;
}


bool CInformationManager::cmd_d(char chOperation)
{
  if (m_daryPstrCommands.get_array_element_count() < 2)
  {
    return false;
  }

  int nID = atoi(**(m_daryPstrCommands[1]));

  (*(objTemplateForQuery.m_pContentPtr)).set_ID(nID);

  CGYAVLTree<CShellForUsingPtrInTree<CGeneralizedObject>>::CGYAVLTreeNode
    *pTreeNode = NULL;

  switch (chOperation)
  {
  case 'd':
    pTreeNode = m_treeStudent.find_node(objTemplateForQuery);
    break;
  case 'D':
    pTreeNode = m_treeCourse.find_node(objTemplateForQuery);
  default:
    break;
  }

  if (pTreeNode == NULL)
  {
    append_output_buffer(INTERFACE_DELETE_ERROR_INFO);
  }
  else
  {
    CShellForUsingPtrInTree<CGeneralizedObject> tmp = (*pTreeNode).m_data;
    CGeneralizedObject *pObj = tmp.m_pContentPtr;
    
    if (!(pObj->when_being_deleted(&m_strOutputBuffer, OUTPUT_BUFFER_LENGTH)))
    {
      append_output_buffer(INTERFACE_DELETE_ERROR_INFO);
    }
  }

  append_output_buffer(INTERFACE_DELETE_SUCCESS_INFO);

  return true;
}


bool CInformationManager::cmd_c(int cmd1, int cmd2)
{
  int nStuID = cmd1;
  int nCourseID = cmd2;

  (*(objTemplateForQuery.m_pContentPtr)).set_ID(nStuID);

  CGYAVLTree<CShellForUsingPtrInTree<CGeneralizedObject>>::CGYAVLTreeNode
    *pTreeNode1 = m_treeStudent.find_node(objTemplateForQuery);

  if (pTreeNode1 == NULL)
  {
    return false;
  }

  CShellForUsingPtrInTree<CGeneralizedObject> tmp1 = (*pTreeNode1).m_data;
  CGeneralizedObject *pObj1 = tmp1.m_pContentPtr;


  (*(objTemplateForQuery.m_pContentPtr)).set_ID(nCourseID);
 
  CGYAVLTree<CShellForUsingPtrInTree<CGeneralizedObject>>::CGYAVLTreeNode
    *pTreeNode2 = m_treeCourse.find_node(objTemplateForQuery);

  if (pTreeNode2 == NULL)
  {
    return false;
  }

  CShellForUsingPtrInTree<CGeneralizedObject> tmp2 = (*pTreeNode2).m_data;
  CGeneralizedObject *pObj2 = tmp2.m_pContentPtr;

  char szEventMsgBuffer[MAX_COMMAND_LINE_LENGTH] = { 0 };

  sprintf(szEventMsgBuffer, "\"c#%d#%d#%d#\"", nStuID, nCourseID, rand()%100);

  CGYObjFactory<CGYString> *pStringFactory = GET_SINGLE(
    CGYObjFactory<CGYString>);
  CGYString *pStrTmp = pStringFactory->get_an_object();

  *pStrTmp = szEventMsgBuffer;

  int nEventID = cmd_a('E', pStrTmp);


  pObj1->add_event(nEventID);
  //�˳���ʱ���
  pObj1->when_being_added_an_event(/*&m_strOutputBuffer, OUTPUT_BUFFER_LENGTH*/);
  pObj2->add_event(nEventID);
  pObj2->when_being_added_an_event(/*&m_strOutputBuffer, OUTPUT_BUFFER_LENGTH*/);

  return true;
}


CGeneralizedObject * CInformationManager::query_event(int nIndex)
{
  return m_EventArray[nIndex];
}


CGYString * CInformationManager::get_name_via_ID(eDataStructureName eName, int nID)
{
  (*(objTemplateForQuery.m_pContentPtr)).set_ID(nID);

  CGYAVLTree<CShellForUsingPtrInTree<CGeneralizedObject>>::CGYAVLTreeNode
    *pTreeNode = NULL;

  switch (eName)
  {
  case CInformationManager::STUDENT_DATA:
    pTreeNode = m_treeStudent.find_node(objTemplateForQuery);
    break;
  case CInformationManager::COURSE_DATA:
    pTreeNode = m_treeCourse.find_node(objTemplateForQuery);
    break;
  default:
    break;
  }

  CShellForUsingPtrInTree<CGeneralizedObject> tmp = (*pTreeNode).m_data;

  CGeneralizedObject *pObj = tmp.m_pContentPtr;

  return pObj->get_name();
}


bool CInformationManager::load_static_serials()
{
  CGYFileOperator objFileOperator(FILE_STATIC_SERIAL);
  char *pchBuffer = NULL;
  objFileOperator.map_file_to_memory(&pchBuffer);

  CGYDynamicArray<CGYString *> tmpAry;
  CGYString::split_string_into_dynamic_array(pchBuffer, tmpAry, '#');

  CEventFYJC::nIDSerialCounter = strtol(**(tmpAry[0]), NULL, 16);
  CStudent::nIDSerialCounter = strtol(**(tmpAry[1]), NULL, 16);
  CCourse::nIDSerialCounter = strtol(**(tmpAry[2]), NULL, 16);

  return true;
}

bool CInformationManager::save_static_serials()
{
  CGYFileOperator objFileOperator(FILE_STATIC_SERIAL);

  char szTmp[MAX_COMMAND_LINE_LENGTH] = { 0 };
  
  sprintf(szTmp, "%08x#%08x#%08x#",
    CEventFYJC::nIDSerialCounter,
    CStudent::nIDSerialCounter,
    CCourse::nIDSerialCounter);

  objFileOperator.write_string_into_file(szTmp, 0);

  return false;
}

bool CInformationManager::cmd_x(int cmd1, int cmd2)
{
  for (int i = 0; i < cmd2; i++)
  {
    cmd_c(cmd1, rand() % CCourse::nIDSerialCounter);
  }

  return false;
}

bool CInformationManager::cmd_z(int cmd1, int cmd2)
{
  for (int i = 0; i < cmd1; i++)
  {
    cmd_x(i, cmd2);
  }

  return false;
}


bool CInformationManager::cmd_a_special_edition_for_loading_from_files(
  int eName, CGYDynamicArray<CGYString *> & objStorageArray, long lOffsetInRoster, char cIsValid)
{
  CGYObjFactory<CGYString> *pStringFactory = GET_SINGLE(
    CGYObjFactory<CGYString>);
  CGYString *pStrTmp = NULL;

  CStudent *pStudentTmp = NULL;
  CCourse *pCourseTmp = NULL;
  CEventFYJC *pEventFYJC = NULL;
  CGeneralizedObject *pGeneralizedObj = NULL;

  int nID = atoi(**(objStorageArray[0]));
  pStrTmp = pStringFactory->get_an_object();
  (*pStrTmp) = **(objStorageArray[1]);

  switch (eName)
  {
  case CInformationManager::STUDENT_DATA:
    pStudentTmp = new CStudent(pStrTmp);
    pStudentTmp->set_ID(nID);
    pStudentTmp->set_RosterOffset(lOffsetInRoster);
    pStudentTmp->set_is_valid(cIsValid);

    pStrTmp = pStringFactory->get_an_object();
    *pStrTmp = **(objStorageArray[2]);

    pStudentTmp->set_comments(pStrTmp);

    pGeneralizedObj = pStudentTmp;
    add_into_data_storage(STUDENT_DATA, pGeneralizedObj);
    break;
  case CInformationManager::COURSE_DATA:
    pCourseTmp = new CCourse(pStrTmp);
    pCourseTmp->set_ID(nID);
    pCourseTmp->set_RosterOffset(lOffsetInRoster);
    pCourseTmp->set_is_valid(cIsValid);

    pGeneralizedObj = pCourseTmp;
    add_into_data_storage(COURSE_DATA, pGeneralizedObj);
    break;
  case CInformationManager::EVENTFYJC_DATA:
    pEventFYJC = new CEventFYJC(pStrTmp);
    pEventFYJC->set_ID(nID);
    pEventFYJC->set_RosterOffset(lOffsetInRoster);
    pEventFYJC->set_is_valid(cIsValid);

    pGeneralizedObj = pEventFYJC;
    add_into_data_storage(EVENTFYJC_DATA, pGeneralizedObj);
    break;
  default:
    break;
  }

  m_aryLoadedFromFile.insert_element(pGeneralizedObj);

  if (eName == STUDENT_DATA)
  {
    for (int i = /*2*/3; i < objStorageArray.get_array_element_count(); i++)
    {
      pGeneralizedObj->add_event(atoi(**(objStorageArray[i])));
    }
  }
  else if (eName == COURSE_DATA)
  {
    for (int i = 2; i < objStorageArray.get_array_element_count(); i++)
    {
      pGeneralizedObj->add_event(atoi(**(objStorageArray[i])));
    }
  }
  

  return true;
}

/*int CInformationManager::cmd_a(char chOperation, CGYString * cmd1)
{
  CGYObjFactory<CGYString> *pStringFactory = GET_SINGLE(
    CGYObjFactory<CGYString>);
  CGYString *pStrTmp = NULL;

  CGeneralizedObject *pGeneralizedObj = NULL;

  switch (chOperation)
  {
  case 'a':
    {
      CGYObjFactory<CStudent> *pStudentFactory = GET_SINGLE(
        CGYObjFactory<CStudent>);
      CStudent *pStuTmp = pStudentFactory->get_an_object();

      add_into_data_storage(CInformationManager::STUDENT_DATA, pStuTmp);
      pGeneralizedObj = pStuTmp;
    }
    break;
  case 'A':
    {
      CGYObjFactory<CCourse> *pCourseFactory = GET_SINGLE(
        CGYObjFactory<CCourse>);
      CCourse *pCourseTmp = pCourseFactory->get_an_object();

      add_into_data_storage(CInformationManager::COURSE_DATA, pCourseTmp);
      pGeneralizedObj = pCourseTmp;
    }
    break;
  case 'E':
    {
    CGYObjFactory<CEventFYJC> *pEventFYJCFactory = GET_SINGLE(
      CGYObjFactory<CEventFYJC>);
    CEventFYJC *pEventFYJCTmp = pEventFYJCFactory->get_an_object();

    add_into_data_storage(CInformationManager::EVENTFYJC_DATA, pEventFYJCTmp);
    pGeneralizedObj = pEventFYJCTmp;
    }
    break;
  default:
    break;
  }

  if (m_daryPstrCommands.get_array_element_count() >= 2 && cmd1 != NULL)
  {
    remove_cmd_quoters(cmd1);
    pStrTmp = pStringFactory->get_an_object();
    pStrTmp->set_string(**cmd1);
    pGeneralizedObj->set_name(pStrTmp);
  }

//   if (m_daryPstrCommands.get_array_element_count() >= 3 && m_daryPstrCommands[2] != NULL)
//   {
//     remove_cmd_quoters(m_daryPstrCommands[2]);
//
//     pStrTmp = pStringFactory->get_an_object();
//     pStrTmp->set_string(**(m_daryPstrCommands[2]));
//     CStudent *pStuTmp = (CStudent *)pGeneralizedObj;  �ؼ�������
//     pStuTmp->set_comments(pStrTmp);
//   }


  pGeneralizedObj->when_being_added(&m_strOutputBuffer, OUTPUT_BUFFER_LENGTH);
  append_output_buffer(INTERFACE_ADD_SUCCESS_INFO);

  return pGeneralizedObj->get_ID();
}
*/

//ʱ��ִ٣��˺���д�ıȽϲ���
int CInformationManager::load_info()
{
  CGYFileOperator *pRoster = new CGYFileOperator("data_files\\student\\roster\\roster.txt");
  CGYFileOperator *pData = new CGYFileOperator("data_files\\student\\data\\data.txt");
  char chTmp = 'a';

  long lRosterFileSize = pRoster->get_file_size_byte();
  long lCurrentRosterOffset = 0;
  char szDataOffset[9] = { 0 };
  char szDataLength[7] = { 0 };


  long lCurrentDataOffset = 0;
  int nCurrentDataLength = 0;

  char *pchRosterBuffer;
  char *pchDataBuffer;

  CGYDynamicArray<CGYString *> daryDataBuffer;

  while (lCurrentRosterOffset < lRosterFileSize)
  {
    char chIsValid = 'T';
    pRoster->map_file_to_memory(&pchRosterBuffer, lCurrentRosterOffset, 16, false);

    if (pchRosterBuffer[0] == 'F')
    {
      /*lCurrentRosterOffset += 16;
      continue;*/
      chIsValid = 'F';
    }

    chTmp = pchRosterBuffer[9];
    pchRosterBuffer[9] = '\0';

    /*for (int i = 0; i < 8; i++)
    {
    szDataOffset[i] = pchRosterBuffer[1 + i];
    }

    szDataOffset[8] = '\0';*/

    lCurrentDataOffset = strtol(pchRosterBuffer + 1, NULL, 16);
    pchRosterBuffer[9] = chTmp;


    /*for (int i = 0; i < 6; i++)
    {
    szDataLength[i] = pchRosterBuffer[i + 9];
    }
    szDataLength[6] = '\0';
    */

    nCurrentDataLength = atoi(pchRosterBuffer + 9);

    if (pchRosterBuffer != NULL)
    {
      delete[]pchRosterBuffer;
      pchRosterBuffer = NULL;
    }
    lCurrentRosterOffset += 16;

    pData->map_file_to_memory(&pchDataBuffer, lCurrentDataOffset, nCurrentDataLength, false);

    daryDataBuffer.empty_array();

    CGYString::split_string_into_dynamic_array(pchDataBuffer, daryDataBuffer, '#');

    cmd_a_special_edition_for_loading_from_files(STUDENT_DATA, daryDataBuffer, lCurrentRosterOffset - 16, chIsValid);
  }


  pRoster = new CGYFileOperator("data_files\\course\\roster\\roster.txt");
  pData = new CGYFileOperator("data_files\\course\\data\\data.txt");
  lRosterFileSize = pRoster->get_file_size_byte();
  lCurrentRosterOffset = 0;
  while (lCurrentRosterOffset < lRosterFileSize)
  {
    char chIsValid = 'T';
    pRoster->map_file_to_memory(&pchRosterBuffer, lCurrentRosterOffset, 16, false);

    if (pchRosterBuffer[0] == 'F')
    {
      /*lCurrentRosterOffset += 16;
      continue;*/
      chIsValid = 'F';
    }

    chTmp = pchRosterBuffer[9];
    pchRosterBuffer[9] = '\0';

    /*for (int i = 0; i < 8; i++)
    {
    szDataOffset[i] = pchRosterBuffer[1 + i];
    }

    szDataOffset[8] = '\0';*/

    lCurrentDataOffset = strtol(pchRosterBuffer + 1, NULL, 16);
    pchRosterBuffer[9] = chTmp;


    /*for (int i = 0; i < 6; i++)
    {
    szDataLength[i] = pchRosterBuffer[i + 9];
    }
    szDataLength[6] = '\0';
    */

    nCurrentDataLength = atoi(pchRosterBuffer + 9);

    if (pchRosterBuffer != NULL)
    {
      delete[]pchRosterBuffer;
      pchRosterBuffer = NULL;
    }
    lCurrentRosterOffset += 16;

    pData->map_file_to_memory(&pchDataBuffer, lCurrentDataOffset, nCurrentDataLength, false);

    daryDataBuffer.empty_array();

    CGYString::split_string_into_dynamic_array(pchDataBuffer, daryDataBuffer, '#');

    cmd_a_special_edition_for_loading_from_files(COURSE_DATA, daryDataBuffer, lCurrentRosterOffset - 16, chIsValid);
  }

  pRoster = new CGYFileOperator("data_files\\eventFYJC\\roster\\roster.txt");
  pData = new CGYFileOperator("data_files\\eventFYJC\\data\\data.txt");
  lRosterFileSize = pRoster->get_file_size_byte();
  lCurrentRosterOffset = 0;
  while (lCurrentRosterOffset < lRosterFileSize)
  {
    char chIsValid = 'T';
    pRoster->map_file_to_memory(&pchRosterBuffer, lCurrentRosterOffset, 16, false);

    if (pchRosterBuffer[0] == 'F')
    {
      /*lCurrentRosterOffset += 16;
      continue;*/
      chIsValid = 'F';
    }

    chTmp = pchRosterBuffer[9];
    pchRosterBuffer[9] = '\0';

    /*for (int i = 0; i < 8; i++)
    {
    szDataOffset[i] = pchRosterBuffer[1 + i];
    }

    szDataOffset[8] = '\0';*/

    lCurrentDataOffset = strtol(pchRosterBuffer + 1, NULL, 16);
    pchRosterBuffer[9] = chTmp;


    /*for (int i = 0; i < 6; i++)
    {
    szDataLength[i] = pchRosterBuffer[i + 9];
    }
    szDataLength[6] = '\0';
    */

    nCurrentDataLength = atoi(pchRosterBuffer + 9);

    if (pchRosterBuffer != NULL)
    {
      delete[]pchRosterBuffer;
      pchRosterBuffer = NULL;
    }
    lCurrentRosterOffset += 16;

    pData->map_file_to_memory(&pchDataBuffer, lCurrentDataOffset, nCurrentDataLength, false);

    daryDataBuffer.empty_array();

    CGYString::split_string_into_dynamic_array2(pchDataBuffer, daryDataBuffer, '#');

    cmd_a_special_edition_for_loading_from_files(EVENTFYJC_DATA, daryDataBuffer, lCurrentRosterOffset - 16, chIsValid);
  }

  return 0;

  return 0;
}
