#pragma once

#include "GYSingleInstanceMacro.h"
#include "GYAVLTree.h"
#include "GeneralizedObject.h"
#include "ShellForUsingPtrInTree.h"
#include "GYString.h"
#include "GYDynamicArray.h"
#include "GYFileOperator.h"
#include "GYHashTable.h"

class CGeneralizedObject;
//������
class CInformationManager
{
public:
#define FILE_STATIC_SERIAL "data_files\\info_manager\\static_serial.txt"
#define MAX_COMMAND_LINE_LENGTH 2048
#define COMMAND_LINE_FORMAT_RESTRICTION "%2047[^\n]s"
#define OUTPUT_BUFFER_LENGTH 2048

#define INTERFACE_WELCOME_INFO "��ӭʹ�ã�������ָ�\r\n\r\n"
#define INTERFACE_INPUT_ERROR_INFO "ָ���ʽ����\r\n\r\n"
#define INTERFACE_PROCESS_ERROR_INFO "ָ��ִ�д���\r\n\r\n"
#define INTERFACE_LIST_ERROR_INFO "û�з��ϲ�ѯ�����Ľ����\r\n\r\n"
#define INTERFACE_LIST_SUCCESS_INFO "��ѯ�ɹ�������\r\n\r\n"
#define INTERFACE_ADD_SUCCESS_INFO "��ӳɹ���\r\n\r\n"
#define INTERFACE_DELETE_ERROR_INFO "û�з���ɾ�������Ľ����\r\n\r\n"
#define INTERFACE_DELETE_SUCCESS_INFO "ɾ���ɹ���\r\n\r\n"
#define INTERFACE_MODIFY_ERROR_INFO "�޸�ʧ�ܣ�ָ����ID��Ч��\r\n\r\n"
#define INTERFACE_MODIFY_SUCCESS_INFO "�޸ĳɹ���\r\n\r\n"



  enum eDataStructureName
  {
    STUDENT_DATA,
    COURSE_DATA,
    EVENTFYJC_DATA
  };

public:
  SINGLE_INSTANCE(CInformationManager)

private:
  CInformationManager();
  virtual ~CInformationManager();

public:
  int add_into_data_storage(eDataStructureName eName, CGeneralizedObject *pObj);

private:
  int init();
  

private:
  //ѧ����Ϣ��
  CGYAVLTree<CShellForUsingPtrInTree<CGeneralizedObject>> m_treeStudent;
  //ѧ������hashtable
  CGYHashTable<int> m_hashtableStudentName;

  //�γ���Ϣ��
  CGYAVLTree<CShellForUsingPtrInTree<CGeneralizedObject>> m_treeCourse;
  //�γ���hashtable
  CGYHashTable<int> m_hashtableCourseName;

  //��Ϣ��̬����
  CGYDynamicArray<CGeneralizedObject *> m_EventArray;

  //���ļ�������Ķ����ڴ˱��ܡ����������д����Ķ�����CGYObjFactoryͳһ�������
  CGYDynamicArray<CGeneralizedObject *> m_aryLoadedFromFile;

  //��̬���飬���ڱ���������û�ָ��
  CGYDynamicArray<CGYString *> m_daryPstrCommands;

  //�û�ָ��buffer�����ڱ���δ��������û�ԭʼָ��
  char m_chStrCommand[MAX_COMMAND_LINE_LENGTH];

  //����scanf()�����ĸ�ʽ��������
  CGYString m_strCommandFormatRestriction;

  //�����ݴ����������ݲ�����������Ҫ��������Ϣ���ȴ����ݲ���������ͳһ����
  CGYString m_strOutputBuffer;

  CShellForUsingPtrInTree<CGeneralizedObject> objTemplateForQuery;

public:
  int main_loop();
private:
  int get_user_command();
  bool parser_user_command();
  int post_posses_of_char_01(char * pszSubCmd);
  bool process_user_command();
  int cmd_a(char chOperation, CGYString * cmd1);
  int remove_cmd_quoters(CGYString * pStrCmd);
  int append_output_buffer(const char * pStrBuffer);
  bool cmd_l(char chSpecificObj_stu_or_course);
  bool cmd_l_num(char chSpecificObj_stu_or_course, int nID);
  CGYString * find_name_in_tree_via_ID(char chSpecifyWhichTreeToLookInto, int nID);
  bool cmd_g(char chOperation);
  bool cmd_d(char chOperation);
  bool cmd_m(char chOperation);
  bool cmd_c(int cmd1, int cmd2);
  bool cmd_x(int cmd1, int cmd2);
  bool cmd_z(int cmd1, int cmd2);

public:
  CGeneralizedObject * query_event(int nIndex);
  bool cmd_a_special_edition_for_loading_from_files(int eName,
    CGYDynamicArray<CGYString *> & objStorageArray);
  CGYString * get_name_via_ID(eDataStructureName eName, int nID);
  bool load_static_serials();

private:
  //���ļ�����ID������
  bool save_static_serials();
  
public:
  int load_info();
};

