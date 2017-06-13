#include "stdafx.h"
#include "EventFYJC.h"
#include "InformationManager.h"

int CEventFYJC::nIDSerialCounter = 0;

CEventFYJC::CEventFYJC()
  :CGeneralizedObject(nIDSerialCounter++, NULL)
{
}

//带构造参数：名字
CEventFYJC::CEventFYJC(CGYString *pStrName)
  : CGeneralizedObject(nIDSerialCounter++, pStrName)
{

}

CEventFYJC::~CEventFYJC()
{
}

int CEventFYJC::init_when_program_startup_eventFYJC()
{
  CGYFileOperator *pRoster = new CGYFileOperator(FILE_ROSTER);
  CGYFileOperator *pData = new CGYFileOperator(FILE_DATA);

  //文件原始数据载入到内存
  //init_load_file_data_to_memory(CInformationManager::STUDENT_DATA, pRoster, pData);

  return 0;
}

bool CEventFYJC::mount_files(CGYFileOperator ** pobjFileOperatorRoster,
  CGYFileOperator ** pobjFileOperatorData)
{
  *pobjFileOperatorRoster = new CGYFileOperator(FILE_ROSTER);
  *pobjFileOperatorData = new CGYFileOperator(FILE_DATA);

  return false;
}

bool CEventFYJC::when_being_added(CGYString * pStrOutputBuffer,
  int nOutputBufferSize)
{
  /*if (pStrOutputBuffer->get_string_length() < nOutputBufferSize)
  {
    char szBasicInfo[MAX_INFORMATION_SIZE] = { 0 };
    say_my_basic_information(szBasicInfo);
    *pStrOutputBuffer += szBasicInfo;
  }*/

  save_object_to_files();

  return true;
}