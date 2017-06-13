#pragma once
#include "GeneralizedObject.h"

//�γ��࣬������Сʵ�ַ�����û�ж�����룩
class CCourse :
  public CGeneralizedObject
{
public:
#define FILE_NAME "txt_files\\course\\names\\name.txt"
#define FILE_ROSTER "data_files\\course\\roster\\roster.txt"
#define FILE_DATA "data_files\\course\\data\\data.txt"

public:
  CCourse();
  CCourse(CGYString *pStrName);
  virtual ~CCourse();

public:
  static int init_when_program_startup_course();

private:
  CGYString * generate_random_name();
  /*==virtual==*/ bool mount_files(CGYFileOperator ** pobjFileOperatorRoster,
    CGYFileOperator ** pobjFileOperatorData);

public:
  static int nIDSerialCounter;
  //��̬�������ڱ�����������Ϣ�����֣����۵ȣ�
  static CGYDynamicArray<CGYString *> *m_pdaryPstrCourseName;
};

