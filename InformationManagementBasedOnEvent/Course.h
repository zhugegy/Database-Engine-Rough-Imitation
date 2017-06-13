#pragma once
#include "GeneralizedObject.h"

//课程类，子类最小实现范例（没有多余代码）
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
  //动态数组用于保存各种随机信息（名字，评论等）
  static CGYDynamicArray<CGYString *> *m_pdaryPstrCourseName;
};

