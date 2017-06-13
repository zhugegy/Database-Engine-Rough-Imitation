#include "stdafx.h"
#include "Course.h"
#include <stdlib.h>
#include "InformationManager.h"

int CCourse::nIDSerialCounter = 0;
CGYDynamicArray<CGYString *> * CCourse::m_pdaryPstrCourseName;

//不带构造参数的情况
CCourse::CCourse()
  :CGeneralizedObject(nIDSerialCounter++, generate_random_name())
{

}

//带构造参数：名字
CCourse::CCourse(CGYString *pStrName)
  :CGeneralizedObject(nIDSerialCounter++, pStrName)
{

}

CCourse::~CCourse()
{
}

int CCourse::init_when_program_startup_course()
{
  m_pdaryPstrCourseName = new CGYDynamicArray<CGYString *>;

  //载入信息到随机姓名（姓）动态数组
  init_load_random_information_to_dynamic_array(FILE_NAME, 
    *m_pdaryPstrCourseName);

  CGYFileOperator *pRoster = new CGYFileOperator(FILE_ROSTER);
  CGYFileOperator *pData = new CGYFileOperator(FILE_DATA);

  //文件原始数据载入到内存
  //init_load_file_data_to_memory(CInformationManager::STUDENT_DATA, pRoster, pData);

  return 0;
}


CGYString * CCourse::generate_random_name()
{
  CGYObjFactory<CGYString> *pStrFactory = GET_SINGLE(CGYObjFactory<CGYString>);

  CGYString *pCourseName = pStrFactory->get_an_object();
  pCourseName->set_string("");

  int nIndex = rand() % (*m_pdaryPstrCourseName).get_array_element_count();
  *pCourseName += (CGYString) *((*m_pdaryPstrCourseName)[nIndex]);

  return pCourseName;
}


bool CCourse::mount_files(CGYFileOperator ** pobjFileOperatorRoster, 
  CGYFileOperator ** pobjFileOperatorData)
{
  *pobjFileOperatorRoster = new CGYFileOperator(FILE_ROSTER);
  *pobjFileOperatorData = new CGYFileOperator(FILE_DATA);

  return false;
}
