#pragma once
#include "GeneralizedObject.h"
class CEventFYJC :
  public CGeneralizedObject
{
public:
#define FILE_ROSTER "data_files\\eventFYJC\\roster\\roster.txt"
#define FILE_DATA "data_files\\eventFYJC\\data\\data.txt"

public:
  CEventFYJC();
  CEventFYJC(CGYString *pStrName);
  virtual ~CEventFYJC();

public:
  static int init_when_program_startup_eventFYJC();

private:
  bool mount_files(CGYFileOperator ** pobjFileOperatorRoster,
    CGYFileOperator ** pobjFileOperatorData);
  bool when_being_added(CGYString * pStrOutputBuffer, int nOutputBufferSize);
public:
  static int nIDSerialCounter;
};

