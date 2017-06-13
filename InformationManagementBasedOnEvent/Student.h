#pragma once
#include "GeneralizedObject.h"
#include "GYString.h"
#include "GYDynamicArray.h"

class CStudent :
  public CGeneralizedObject
{
public:
  //必写 MUST
#define FILE_FAMILY_NAME "txt_files\\student\\names\\family_name.txt"
  //可写 OPTIONAL
#define FILE_GIVEN_NAME "txt_files\\student\\names\\given_name.txt"
  //可写 OPTIONAL
#define FILE_COMMENTS "txt_files\\student\\comments\\comments.txt"
  //必写 MUST
#define FILE_ROSTER "data_files\\student\\roster\\roster.txt"
  //必写 MUST
#define FILE_DATA "data_files\\student\\data\\data.txt"

public:
  //必写 MUST
  CStudent();
  //必写 MUST
  CStudent(CGYString *pStrName);
  //可写 OPTIONAL
  CStudent(CGYString *pStrName, CGYString *pStrComments);
  //必写 MUST
  virtual ~CStudent();

public:
  //必写 MUST
  static int init_when_program_startup_student();

public:
  //可写 OPTIONAL
  bool set_comments(CGYString * pStrComments);
  //可写 OPTIONAL
  CGYString * get_comments();

private:
  //必写 MUST
  CGYString * generate_random_student_name();
  //可写 OPTIONAL
  CGYString * generate_random_student_comments();

  //可写 OPTIONAL 可用基类默认
  /*==virtual==*/ int say_my_basic_information(char * pStrBuffer);
  //必写 MUST 基类是空函数，必须指定自己的相关存储文件名
  /*==virtual==*/ bool mount_files(CGYFileOperator ** pobjFileOperatorRoster, CGYFileOperator ** pobjFileOperatorData);
  //可写 OPTIONAL 可用基类默认
  /*==virtual==*/ int encode_data_file_string(CGYString * pStrBuffer);

public:
  //必写 MUST
  static int nIDSerialCounter;
  //动态数组用于保存各种随机信息（名字，评论等）
  //必写 MUST
  static CGYDynamicArray<CGYString *> *m_pdaryPstrStudentFamilyName;
  //可写 OPTIONAL
  static CGYDynamicArray<CGYString *> *m_pdaryPstrStudentGivenName;
  //可写 OPTIONAL
  static CGYDynamicArray<CGYString *> *m_pdaryPstrStudentComments;

private:
  //可写 OPTIONAL
  CGYString *m_pStrComments;
};

