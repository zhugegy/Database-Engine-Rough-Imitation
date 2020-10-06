#include "stdafx.h"
#include "GeneralizedObject.h"
#include <stdlib.h>
#include "InformationManager.h"
#include "GYSingleInstanceMacro.h"

CGYLinkList<CGeneralizedObject *> CGeneralizedObject::pAllObjects;

CGeneralizedObject::CGeneralizedObject(int nID, CGYString *pStrName)
{
  add_ptr_to_all_obj_lst();

  m_nID = nID;
  m_pStrName = pStrName;
  m_lOffsetInRoster = -1;
  m_chIsValid = 'T';
}


CGeneralizedObject::~CGeneralizedObject()
{

}


bool CGeneralizedObject::add_event(int lEventID)
{
  m_lstEvents.insert_to_tail(lEventID);

  return true;
}


bool CGeneralizedObject::add_ptr_to_all_obj_lst()
{
  pAllObjects.insert_to_tail(this);

  return false;
}

bool CGeneralizedObject::save_object_to_files()
{
  if (m_chIsValid == 'F')
  {
    return false;
  }

  CGYFileOperator * pobjFileOperatorRoster = NULL;
  CGYFileOperator * pobjFileOperatorData = NULL;

  mount_files(&pobjFileOperatorRoster, &pobjFileOperatorData);

  /*char szDataBuffer[MAX_INFORMATION_SIZE];*/
  CGYObjFactory<CGYString> *pStringFactory = GET_SINGLE(
    CGYObjFactory<CGYString>);
  CGYString *pStrDataBuffer = pStringFactory->get_an_object();
  pStrDataBuffer->set_string("");

  CGYString *pStrRosterBuffer = pStringFactory->get_an_object();
  pStrRosterBuffer->set_string("");

  char chIsValid = 'T';
  long lOffsetInData = 0;
  int nDataLength = encode_data_file_string(pStrDataBuffer);

  if (m_lOffsetInRoster == -1)
  {
    m_lOffsetInRoster = pobjFileOperatorRoster->get_file_size_byte();

    lOffsetInData = pobjFileOperatorData->get_file_size_byte();

    pobjFileOperatorData->write_string_into_file(**pStrDataBuffer);

    encode_roster_file_string(pStrRosterBuffer, 'T', lOffsetInData, nDataLength);

    pobjFileOperatorRoster->write_string_into_file(**pStrRosterBuffer);
  }
  else
  {
    int nPrivouisDataLength = 0;
    decode_roster_file_string(pobjFileOperatorRoster, m_lOffsetInRoster, chIsValid, lOffsetInData, nPrivouisDataLength);

    if (nDataLength > nPrivouisDataLength)
    {
      lOffsetInData = pobjFileOperatorData->get_file_size_byte();
      pobjFileOperatorData->write_string_into_file(**pStrDataBuffer);
    }
    else
    {
      pobjFileOperatorData->write_string_into_file(**pStrDataBuffer, lOffsetInData);
    }

    encode_roster_file_string(pStrRosterBuffer, 'T', lOffsetInData, nDataLength);
    pobjFileOperatorRoster->write_string_into_file(**pStrRosterBuffer, m_lOffsetInRoster);
  }

  //deletefileoperator
  if (pobjFileOperatorRoster != NULL)
  {
    delete pobjFileOperatorRoster;
    pobjFileOperatorRoster = NULL;
  }

  if (pobjFileOperatorData != NULL)
  {
    delete pobjFileOperatorData;
    pobjFileOperatorData = NULL;
  }

  return false;
}

bool CGeneralizedObject::mark_obj_as_invalid_in_file_roster_when_it_is_deleted()
{
  CGYFileOperator * pobjFileOperatorRoster = NULL;
  CGYFileOperator * pobjFileOperatorData = NULL;

  mount_files(&pobjFileOperatorRoster, &pobjFileOperatorData);

  char *pszRosterBuffer = NULL;
  pobjFileOperatorRoster->map_file_to_memory(&pszRosterBuffer, 
    m_lOffsetInRoster, 16, false);

  pszRosterBuffer[0] = 'F';

  pobjFileOperatorRoster->write_string_into_file(pszRosterBuffer, 
    m_lOffsetInRoster);

  return true;
}

bool CGeneralizedObject::set_name(CGYString * pStrName)
{
  m_pStrName = pStrName;

  return true;
}


bool CGeneralizedObject::operator>(const CGeneralizedObject & obj)
{
  return m_nID > obj.m_nID;
}


bool CGeneralizedObject::operator<(const CGeneralizedObject & obj)
{
  return m_nID < obj.m_nID;
}

bool CGeneralizedObject::operator==(const CGeneralizedObject & obj)
{
  return m_nID == obj.m_nID;
}

bool CGeneralizedObject::operator!=(const CGeneralizedObject & obj)
{
  return m_nID != obj.m_nID;
}

CGYString * CGeneralizedObject::get_name()
{
  return m_pStrName;
}


int CGeneralizedObject::get_ID()
{
  return m_nID;
}


int CGeneralizedObject::set_ID(int nID)
{
  m_nID = nID;
  return 0;
}

int CGeneralizedObject::set_RosterOffset(long lOffset)
{
  m_lOffsetInRoster = lOffset;
  return 0;
}


int CGeneralizedObject::say_my_basic_information(char * pStrBuffer)
{
  sprintf(pStrBuffer, "ID：%d\r\n名字：%s\r\n\r\n",
    m_nID,
    **m_pStrName);

  return 0;
}

int CGeneralizedObject::say_my_event_information(CGYString * pStrOutputBuffer)
{
  CGYDynamicArray<CGYString *> tmpAry;
  CInformationManager *pInfoManager = GET_SINGLE(CInformationManager);

  char szBuffer[MAX_INFORMATION_SIZE] = { 0 };

  for (CGYLinkList<int>::iterator it = m_lstEvents.begin();
    it != m_lstEvents.end();
    it++)
  {
    tmpAry.empty_array();
    CGeneralizedObject *pEvent = pInfoManager->query_event(*it);
    CGYString::split_string_into_dynamic_array(**(pEvent->get_name()), tmpAry, '#');

    //switch case (*(tmpAry[0]))[0] 这个代表事件类型
    switch ((*(tmpAry[0]))[0])
    {
    case 'c':
    {
      sprintf(szBuffer, "选课信息：学生 %d %s 选择 课程 %d %s 成绩 %d\r\n",
        atoi(**(tmpAry[1])),
        **(pInfoManager->get_name_via_ID(CInformationManager::STUDENT_DATA,
          atoi(**(tmpAry[1])))),
        atoi(**(tmpAry[2])),
        **(pInfoManager->get_name_via_ID(CInformationManager::COURSE_DATA,
          atoi(**(tmpAry[2])))),
        atoi(**(tmpAry[3])));

      *pStrOutputBuffer += szBuffer;
    }
      break;
    default:
      break;
    }

    
  }

  *pStrOutputBuffer += "\r\n";

  return 0;
}


bool CGeneralizedObject::set_is_valid(char chStatus)
{
  m_chIsValid = chStatus;
  
  return true;
}


char CGeneralizedObject::get_is_valid()
{
  return m_chIsValid;
}

int CGeneralizedObject::init_load_random_information_to_dynamic_array(
  const char * szFileName, CGYDynamicArray<CGYString *> & dary,
  const char delim)
{
  //用于把文件映射到内存的内存块
  char *pchFileToMemoryBuffer = NULL;

  //文件操作类指针
  CGYFileOperator * pobjFileOperator = new CGYFileOperator(szFileName);
  pobjFileOperator->map_file_to_memory(&pchFileToMemoryBuffer);
  CGYString::split_string_into_dynamic_array(pchFileToMemoryBuffer, dary,
    delim);

  if (pchFileToMemoryBuffer != NULL)
  {
    delete[] pchFileToMemoryBuffer;
    pchFileToMemoryBuffer = NULL;
  }
  if (pobjFileOperator != NULL)
  {
    delete pobjFileOperator;
    pobjFileOperator = NULL;
  }

  return 0;
}

bool CGeneralizedObject::when_being_added(CGYString * pStrOutputBuffer, 
  int nOutputBufferSize)
{
  if (pStrOutputBuffer->get_string_length() < nOutputBufferSize)
  {
    char szBasicInfo[MAX_INFORMATION_SIZE] = { 0 };
    say_my_basic_information(szBasicInfo);
    *pStrOutputBuffer += szBasicInfo;
  }

  save_object_to_files();

  return true;
}

bool CGeneralizedObject::when_being_modified(CGYString * pStrOutputBuffer, 
  int nOutputBufferSize)
{
  when_being_added(pStrOutputBuffer, nOutputBufferSize);

  return true;
}

bool CGeneralizedObject::mount_files(CGYFileOperator ** pobjFileOperatorRoster, 
  CGYFileOperator ** pobjFileOperatorData)
{
  //空，必须由子类重写。没法设为虚函数，因为InfoManager会实例化一个基类的对象用于查找

  return false;
}



int CGeneralizedObject::encode_data_file_string(CGYString * pStrBuffer)
{
  char szBuffer[MAX_INFORMATION_SIZE] = { 0 };

  sprintf(szBuffer, "%d#%s#", m_nID, **m_pStrName);
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


bool CGeneralizedObject::encode_roster_file_string(CGYString * pStrBuffer, char chIsValid, long lOffsetInData, int nDataLength)
{
  char szBuffer[MAX_INFORMATION_SIZE] = { 0 };
  sprintf(szBuffer, "%c%08x%06d", chIsValid, lOffsetInData, nDataLength);
  *pStrBuffer += szBuffer;

  return false;
}


bool CGeneralizedObject::decode_roster_file_string(CGYFileOperator * pobjFileOperatorRoster, long lOffsetInRoster, char & chIsValid, long & lOffsetInData, int & nDataLength)
{
  char *pchBuffer = NULL;

  pobjFileOperatorRoster->map_file_to_memory(&pchBuffer, lOffsetInRoster, 15, true);

  chIsValid = pchBuffer[0];

  char szOffset[9] = { 0 };

  for (int i = 0; i < 8; i++)
  {
    szOffset[i] = pchBuffer[i + 1];
  }
  szOffset[8] = '\0';

  lOffsetInData = strtol(szOffset, NULL, 16);

  char szLength[7] = { 0 };

  for (int i = 0; i < 6; i++)
  {
    szLength[i] = pchBuffer[i + 9];
  }
  szLength[6] = '\0';

  nDataLength = atoi(szLength);


  delete []pchBuffer;

  return true;
}


bool CGeneralizedObject::when_being_listed(CGYString * pStrOutputBuffer, 
  int nOutputBufferSize)
{
  if (m_chIsValid != 'T')
  {
    *pStrOutputBuffer += INTERFACE_LIST_ERROR_INFO_DELETED;
    //append_output_buffer(INTERFACE_LIST_ERROR_INFO);
    return false;
  }

  if (pStrOutputBuffer->get_string_length() < nOutputBufferSize)
  {
    char szBasicInfo[MAX_INFORMATION_SIZE] = { 0 };
    say_my_basic_information(szBasicInfo);
    *pStrOutputBuffer += szBasicInfo;

    say_my_event_information(pStrOutputBuffer);
  }

  return true;
}


bool CGeneralizedObject::when_being_deleted(CGYString * pStrOutputBuffer, 
  int nOutputBufferSize)
{
  if (m_chIsValid == 'F')
  {
    return false;
  }

  if (pStrOutputBuffer->get_string_length() < nOutputBufferSize)
  {
    char szBasicInfo[MAX_INFORMATION_SIZE] = { 0 };
    say_my_basic_information(szBasicInfo);
    *pStrOutputBuffer += szBasicInfo;
  }

  m_chIsValid = 'F';

  mark_obj_as_invalid_in_file_roster_when_it_is_deleted();

  return true;
}







int CGeneralizedObject::init_load_file_data_to_memory(
  int eName,
  CGYFileOperator * pobjFileOperatorRoster,
  CGYFileOperator * pobjFileOperatorData)
{

  long lRosterFileSize = pobjFileOperatorRoster->get_file_size_byte();
  long lCurrentRosterOffset = 0;
  char szDataOffset[9] = { 0 };
  char szDataLength[7] = { 0 };
  CInformationManager *pInfoManager = GET_SINGLE(CInformationManager);

  long lCurrentDataOffset = 0;
  int nCurrentDataLength = 0;

  char *pchRosterBuffer;
  char *pchDataBuffer;

  CGYDynamicArray<CGYString *> daryDataBuffer;

  while (lCurrentRosterOffset < lRosterFileSize)
  {
    char chIsValid = 'T';
    pobjFileOperatorRoster->map_file_to_memory(&pchRosterBuffer, lCurrentRosterOffset, 16, false);

    if (pchRosterBuffer[0] == 'F')
    {
      /*lCurrentRosterOffset += 16;
      continue;*/
      chIsValid = 'F';
    }

    for (int i = 0; i < 8; i++)
    {
      szDataOffset[i] = pchRosterBuffer[1 + i];
    }

    szDataOffset[8] = '\0';

    lCurrentDataOffset = strtol(szDataOffset, NULL, 16);

    for (int i = 0; i < 6; i++)
    {
      szDataLength[i] = pchRosterBuffer[i + 9];
    }
    szDataLength[6] = '\0';

    nCurrentDataLength = atoi(szDataLength);

    if (pchRosterBuffer != NULL)
    {
      delete[]pchRosterBuffer;
      pchRosterBuffer = NULL;
    }
    lCurrentRosterOffset += 16;

    pobjFileOperatorData->map_file_to_memory(&pchDataBuffer, lCurrentDataOffset, nCurrentDataLength, false);

    daryDataBuffer.empty_array();

    CGYString::split_string_into_dynamic_array(pchDataBuffer, daryDataBuffer, '#');

    pInfoManager->cmd_a_special_edition_for_loading_from_files(eName, daryDataBuffer, lCurrentRosterOffset - 16, chIsValid);
  }
  return 0;
}

/*char *pchBuffer = NULL;

  pobjFileOperatorRoster->map_file_to_memory(&pchBuffer, lOffsetInRoster, 15, true);

  chIsValid = pchBuffer[0];

  char szOffset[9] = { 0 };

  for (int i = 0; i < 8; i++)
  {
    szOffset[i] = pchBuffer[i + 1];
  }
  szOffset[8] = '\0';

  lOffsetInData = strtol(szOffset, NULL, 16);

  char szLength[7] = { 0 };

  for (int i = 0; i < 6; i++)
  {
    szLength[i] = pchBuffer[i + 9];
  }
  szLength[6] = '\0';

  nDataLength = atoi(szLength);


  delete []pchBuffer;

  return true;*/

bool CGeneralizedObject::when_being_added_an_event()
{
  save_object_to_files();

  return false;
}
