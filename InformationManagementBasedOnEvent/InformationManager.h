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
//单例类
class CInformationManager
{
public:
#define FILE_STATIC_SERIAL "data_files\\info_manager\\static_serial.txt"
#define MAX_COMMAND_LINE_LENGTH 2048
#define COMMAND_LINE_FORMAT_RESTRICTION "%2047[^\n]s"
#define OUTPUT_BUFFER_LENGTH 2048

#define INTERFACE_WELCOME_INFO "欢迎使用！请输入指令。\r\n\r\n"
#define INTERFACE_INPUT_ERROR_INFO "指令格式错误。\r\n\r\n"
#define INTERFACE_PROCESS_ERROR_INFO "指令执行错误。\r\n\r\n"
#define INTERFACE_LIST_ERROR_INFO "没有符合查询条件的结果。\r\n\r\n"
#define INTERFACE_LIST_SUCCESS_INFO "查询成功结束。\r\n\r\n"
#define INTERFACE_ADD_SUCCESS_INFO "添加成功。\r\n\r\n"
#define INTERFACE_DELETE_ERROR_INFO "没有符合删除条件的结果。\r\n\r\n"
#define INTERFACE_DELETE_SUCCESS_INFO "删除成功。\r\n\r\n"
#define INTERFACE_MODIFY_ERROR_INFO "修改失败，指定的ID无效。\r\n\r\n"
#define INTERFACE_MODIFY_SUCCESS_INFO "修改成功。\r\n\r\n"



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
  //学生信息树
  CGYAVLTree<CShellForUsingPtrInTree<CGeneralizedObject>> m_treeStudent;
  //学生姓名hashtable
  CGYHashTable<int> m_hashtableStudentName;

  //课程信息树
  CGYAVLTree<CShellForUsingPtrInTree<CGeneralizedObject>> m_treeCourse;
  //课程名hashtable
  CGYHashTable<int> m_hashtableCourseName;

  //信息动态数组
  CGYDynamicArray<CGeneralizedObject *> m_EventArray;

  //从文件中载入的对象，在此保管。程序运行中创建的对象，由CGYObjFactory统一负责管理。
  CGYDynamicArray<CGeneralizedObject *> m_aryLoadedFromFile;

  //动态数组，用于保存粉碎后的用户指令
  CGYDynamicArray<CGYString *> m_daryPstrCommands;

  //用户指令buffer，用于保存未经处理的用户原始指令
  char m_chStrCommand[MAX_COMMAND_LINE_LENGTH];

  //用于scanf()函数的格式化操作符
  CGYString m_strCommandFormatRestriction;

  //用于暂存所有在数据操作过程中想要发出的信息，等待数据操作结束后，统一发送
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
  //从文件载入ID计数器
  bool save_static_serials();
  
public:
  int load_info();
};

