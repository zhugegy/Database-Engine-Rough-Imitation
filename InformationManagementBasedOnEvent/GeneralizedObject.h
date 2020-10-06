#pragma once

#include "GYString.h"
#include "GYLinkList.h"
#include "GYFileOperator.h"
#include "InformationManager.h"

/*class CInformationManager;*/

class CGeneralizedObject
{
public:
#define MAX_INFORMATION_SIZE 2048

public:
  CGeneralizedObject(int nID, CGYString *pStrName);
  virtual ~CGeneralizedObject();

public:
  CGYString * get_name();
  bool set_name(CGYString * pStrName);
  int get_ID();
  int set_ID(int nID);
  int set_RosterOffset(long lOffet);
  char get_is_valid();
  bool set_is_valid(char chStatus);
  bool add_event(int event_id);

  virtual bool operator>(const CGeneralizedObject & obj);
  virtual bool operator<(const CGeneralizedObject & obj);
  virtual bool operator==(const CGeneralizedObject & obj);
  virtual bool operator!=(const CGeneralizedObject & obj);

  virtual bool when_being_added(CGYString * pStrOutputBuffer, int nOutputBufferSize);
  bool when_being_listed(CGYString * pStrOutputBuffer, int nOutputBufferSize);
  bool when_being_deleted(CGYString * pStrOutputBuffer, int nOutputBufferSize);
  bool when_being_modified(CGYString * pStrOutputBuffer, int nOutputBufferSize);
  bool save_object_to_files();

protected:    //子类必须重写mount_files()，制定自己的文件存储位置，其他选择性重写
  virtual int say_my_basic_information(char * pStrBuffer);
  virtual int say_my_event_information(CGYString * pStrOutputBuffer);
  virtual bool mount_files(CGYFileOperator ** pobjFileOperatorRoster, 
    CGYFileOperator ** pobjFileOperatorData);
  virtual int encode_data_file_string(CGYString * pStrBuffer);

protected:
  static int init_load_random_information_to_dynamic_array(
    const char * szFileName, CGYDynamicArray<CGYString *> & dary, 
    const char delim = '#');



  static int init_load_file_data_to_memory(
    int eName, 
    CGYFileOperator * pobjFileOperatorRoster, 
    CGYFileOperator * pobjFileOperatorData);

private:
  bool add_ptr_to_all_obj_lst();

  bool encode_roster_file_string(CGYString * pStrBuffer, char chIsValid,
    long lOffsetInData, int nDataLength);
  bool decode_roster_file_string(CGYFileOperator * pobjFileOperatorRoster,
    long lOffsetInRoster, char & chIsValid, long & lOffsetInData,
    int & nDataLength);

public:
  //用于多态，每当子类被实例化为对象，在构造函数中会更新此链表
  static CGYLinkList<CGeneralizedObject *> pAllObjects;

protected:
  int m_nID;
  CGYString *m_pStrName;
  long m_lOffsetInRoster;
  char m_chIsValid;
  CGYLinkList<int> m_lstEvents;
public:
private:
  bool mark_obj_as_invalid_in_file_roster_when_it_is_deleted();
public:
  bool when_being_added_an_event();
};

