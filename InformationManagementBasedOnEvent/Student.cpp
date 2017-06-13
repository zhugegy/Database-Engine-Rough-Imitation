#include "stdafx.h"
#include "Student.h"
#include <stdlib.h>
#include "InformationManager.h"

int CStudent::nIDSerialCounter = 0;
CGYDynamicArray<CGYString *> * CStudent::m_pdaryPstrStudentFamilyName;
CGYDynamicArray<CGYString *> * CStudent::m_pdaryPstrStudentGivenName;
CGYDynamicArray<CGYString *> * CStudent::m_pdaryPstrStudentComments;

int CStudent::init_when_program_startup_student()
{
  m_pdaryPstrStudentFamilyName = new CGYDynamicArray<CGYString *>;
  m_pdaryPstrStudentGivenName = new CGYDynamicArray<CGYString *>;
  m_pdaryPstrStudentComments = new CGYDynamicArray<CGYString *>;

  //载入信息到随机姓名（姓）动态数组
  init_load_random_information_to_dynamic_array(FILE_FAMILY_NAME,
    *m_pdaryPstrStudentFamilyName);

  //载入信息到随机姓名（名）动态数组
  init_load_random_information_to_dynamic_array(FILE_GIVEN_NAME,
    *m_pdaryPstrStudentGivenName);

  //载入信息到随机评价动态数组
  init_load_random_information_to_dynamic_array(FILE_COMMENTS,
    *m_pdaryPstrStudentComments);

  CGYFileOperator *pRoster = new CGYFileOperator(FILE_ROSTER);
  CGYFileOperator *pData = new CGYFileOperator(FILE_DATA);

  //文件原始数据载入到内存
  //init_load_file_data_to_memory(CInformationManager::STUDENT_DATA, pRoster, pData);

  return 0;
}

//不带构造参数的情况
CStudent::CStudent()
  :CGeneralizedObject(nIDSerialCounter++, generate_random_student_name())
{
  m_pStrComments = generate_random_student_comments();
}

//带构造参数：名字
CStudent::CStudent(CGYString *pStrName)
  :CGeneralizedObject(nIDSerialCounter++, pStrName)
{

  m_pStrComments = generate_random_student_comments();
}

//带构造参数：名字，评价
CStudent::CStudent(CGYString *pStrName, CGYString *pStrComments)
  :CGeneralizedObject(nIDSerialCounter++, pStrName)
{
  m_pStrComments = pStrComments;
}


CStudent::~CStudent()
{

}


bool CStudent::set_comments(CGYString * pStrComments)
{
  m_pStrComments = pStrComments;

  return true;
}


CGYString * CStudent::get_comments()
{
  return m_pStrComments;
}


int CStudent::say_my_basic_information(char * pStrBuffer)
{
  sprintf(pStrBuffer, "ID：%d\r\n姓名：%s\r\n评价：%s\r\n\r\n",
    m_nID,
    **m_pStrName,
    **m_pStrComments);
  return 0;
}

CGYString * CStudent::generate_random_student_name()
{
  CGYObjFactory<CGYString> *pStrFactory = GET_SINGLE(CGYObjFactory<CGYString>);

  CGYString *pStudentName = pStrFactory->get_an_object();
  pStudentName->set_string("");

  int nIndex = rand() % (*m_pdaryPstrStudentFamilyName).get_array_element_count();
  *pStudentName += (CGYString) *((*m_pdaryPstrStudentFamilyName)[nIndex]);

  nIndex = rand() % (*m_pdaryPstrStudentGivenName).get_array_element_count();
  *pStudentName += (CGYString) *((*m_pdaryPstrStudentGivenName)[nIndex]);

  return pStudentName;
}

CGYString * CStudent::generate_random_student_comments()
{
  CGYObjFactory<CGYString> *pStrFactory = GET_SINGLE(CGYObjFactory<CGYString>);

  CGYString *pStudentComments = pStrFactory->get_an_object();
  pStudentComments->set_string("");

  int nIndex = rand() % (*m_pdaryPstrStudentComments).get_array_element_count();
  *pStudentComments += (CGYString) *((*m_pdaryPstrStudentComments)[nIndex]);

  return pStudentComments;
}

bool CStudent::mount_files(CGYFileOperator ** pobjFileOperatorRoster, CGYFileOperator ** pobjFileOperatorData)
{
  *pobjFileOperatorRoster = new CGYFileOperator(FILE_ROSTER);
  *pobjFileOperatorData = new CGYFileOperator(FILE_DATA);
  return false;
}

int CStudent::encode_data_file_string(CGYString * pStrBuffer)
{
 /* char szBuffer[MAX_INFORMATION_SIZE] = { 0 };
  char szEventBuffer[MAX_INFORMATION_SIZE] = { 0 };
  char szOverallBuffer[MAX_INFORMATION_SIZE] = { 0 };


  for (CGYLinkList<int>::iterator it = m_lstEvents.begin();
    it != m_lstEvents.end();
    it++)
  {
    sprintf(szBuffer, "%d#", *it);
    strcat(szEventBuffer, szBuffer);
  }



  sprintf(szOverallBuffer, "%d#%s#%s#", m_nID, **m_pStrName, **m_pStrComments);
  strcat(szOverallBuffer, szEventBuffer);

  *pStrBuffer += szBuffer;

  / *sprintf(szBuffer, "%s#", m_nID, **m_pStrComments);
  *pStrBuffer += szBuffer;* /
   
  

  int nStringLength = (*pStrBuffer).get_string_length();

  return nStringLength;*/
  char szBuffer[MAX_INFORMATION_SIZE] = { 0 };

  sprintf(szBuffer, "%d#%s#%s#", m_nID, **m_pStrName, **m_pStrComments);
  *pStrBuffer += szBuffer;

  for (CGYLinkList<int>::iterator it = m_lstEvents.begin();
    it != m_lstEvents.end();
    it++)
  {
    sprintf(szBuffer, "%d#", *it);
    *pStrBuffer += szBuffer;
  }

  int nStringLength = (*pStrBuffer).get_string_length();

  return nStringLength;
}

