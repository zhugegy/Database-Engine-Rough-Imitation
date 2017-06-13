#pragma once
#include "GeneralizedObject.h"
#include "GYString.h"
#include "GYDynamicArray.h"

class CStudent :
  public CGeneralizedObject
{
public:
  //��д MUST
#define FILE_FAMILY_NAME "txt_files\\student\\names\\family_name.txt"
  //��д OPTIONAL
#define FILE_GIVEN_NAME "txt_files\\student\\names\\given_name.txt"
  //��д OPTIONAL
#define FILE_COMMENTS "txt_files\\student\\comments\\comments.txt"
  //��д MUST
#define FILE_ROSTER "data_files\\student\\roster\\roster.txt"
  //��д MUST
#define FILE_DATA "data_files\\student\\data\\data.txt"

public:
  //��д MUST
  CStudent();
  //��д MUST
  CStudent(CGYString *pStrName);
  //��д OPTIONAL
  CStudent(CGYString *pStrName, CGYString *pStrComments);
  //��д MUST
  virtual ~CStudent();

public:
  //��д MUST
  static int init_when_program_startup_student();

public:
  //��д OPTIONAL
  bool set_comments(CGYString * pStrComments);
  //��д OPTIONAL
  CGYString * get_comments();

private:
  //��д MUST
  CGYString * generate_random_student_name();
  //��д OPTIONAL
  CGYString * generate_random_student_comments();

  //��д OPTIONAL ���û���Ĭ��
  /*==virtual==*/ int say_my_basic_information(char * pStrBuffer);
  //��д MUST �����ǿպ���������ָ���Լ�����ش洢�ļ���
  /*==virtual==*/ bool mount_files(CGYFileOperator ** pobjFileOperatorRoster, CGYFileOperator ** pobjFileOperatorData);
  //��д OPTIONAL ���û���Ĭ��
  /*==virtual==*/ int encode_data_file_string(CGYString * pStrBuffer);

public:
  //��д MUST
  static int nIDSerialCounter;
  //��̬�������ڱ�����������Ϣ�����֣����۵ȣ�
  //��д MUST
  static CGYDynamicArray<CGYString *> *m_pdaryPstrStudentFamilyName;
  //��д OPTIONAL
  static CGYDynamicArray<CGYString *> *m_pdaryPstrStudentGivenName;
  //��д OPTIONAL
  static CGYDynamicArray<CGYString *> *m_pdaryPstrStudentComments;

private:
  //��д OPTIONAL
  CGYString *m_pStrComments;
};

