#include "stdafx.h"
#include <windows.h>


#include "ManagementWindow.h"
#include "resource.h"
#include "InformationManager.h"
#include <commctrl.h>

#include "Student.h"
#include "Course.h"

#define STUDENT_LIST_HEIGHT 29
#define COURSE_LIST_HEIGHT 8

int onInit(HWND hwndDlg);


int InsertNewRow2List(HWND hwndDlg, int iListViewID, DWORD dwRowID, LPARAM lParam,
  int iColumnNum, LPTSTR szColumn1, LPTSTR szColumn2, LPTSTR szColumn3);
int SerialAddEntries2List(HWND hwndDlg, int iListControl, bool bDirection,
  int iMaxHeight, LPCTSTR szCmd, int iStartingID, int iMaxID, int iMinID = 0);
int SetPageNum(HWND hwndDlg, bool bDirection, int iListID, int iListMaxHeight, int iEditID);

int onClickButtonConsole(HWND hwndDlg);

int onClickButtonStudentListFirstPage(HWND hwndDlg);
int onClickButtonStudentListPreviousPage(HWND hwndDlg);
int onClickButtonStudentListGoPage(HWND hwndDlg);
int onClickButtonStudentListNextPage(HWND hwndDlg);
int onClickButtonStudentListLastPage(HWND hwndDlg);

int onClickButtonCourseListFirstPage(HWND hwndDlg);
int onClickButtonCourseListPreviousPage(HWND hwndDlg);
int onClickButtonCourseListGoPage(HWND hwndDlg);
int onClickButtonCourseListNextPage(HWND hwndDlg);
int onClickButtonCourseListLastPage(HWND hwndDlg);

CGYString CommandExecute(LPTSTR szCmd);

int onClickButtonFunAddRandom(HWND hwndDlg);
int onClickButtonFunAddWithName(HWND hwndDlg);

int onClickButtonFunDeleteCurrentSelection(HWND hwndDlg);

int onClickButtonFunModifyCurrentSelection(HWND hwndDlg);

int onClickButtonFunQueryCurrentSelection(HWND hwndDlg);
int onClickButtonFunQueryID(HWND hwndDlg);
int onClickButtonFunQueryName(HWND hwndDlg);

int onClickButtonFunStudentSelectsCourse(HWND hwndDlg);




BOOL CALLBACK MainWindowProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_INITDIALOG:
    onInit(hwndDlg);
    return TRUE;
  case WM_COMMAND:
    switch (LOWORD(wParam))
    {
    case IDOK:
      return TRUE;
    case IDCANCEL:
      return TRUE;
    case IDC_BUTTON_STUDENT_LIST_FIRST_PAGE:
      onClickButtonStudentListFirstPage(hwndDlg);
      return TRUE;
    case IDC_BUTTON_STUDENT_LIST_PREVIOUS_PAGE:
      onClickButtonStudentListPreviousPage(hwndDlg);
      return TRUE;
    case IDC_BUTTON_STUDENT_LIST_GO_PAGE:
      onClickButtonStudentListGoPage(hwndDlg);
      return TRUE;
    case IDC_BUTTON_STUDENT_LIST_NEXT_PAGE:
      onClickButtonStudentListNextPage(hwndDlg);
      return TRUE;
    case IDC_BUTTON_STUDENT_LIST_LAST_PAGE:
      onClickButtonStudentListLastPage(hwndDlg);
      return TRUE;
    case IDC_BUTTON_COURSE_LIST_FIRST_PAGE:
      onClickButtonCourseListFirstPage(hwndDlg);
      return TRUE;
    case IDC_BUTTON_COURSE_LIST_PREVIOUS_PAGE:
      onClickButtonCourseListPreviousPage(hwndDlg);
      return TRUE;
    case IDC_BUTTON_COURSE_LIST_GO_PAGE:
      onClickButtonCourseListGoPage(hwndDlg);
      return TRUE;
    case IDC_BUTTON_COURSE_LIST_NEXT_PAGE:
      onClickButtonCourseListNextPage(hwndDlg);
      return TRUE;
    case IDC_BUTTON_COURSE_LIST_LAST_PAGE:
      onClickButtonCourseListLastPage(hwndDlg);
      return TRUE;
    case IDC_BUTTON_FUN_ADD_RANDOM:
      onClickButtonFunAddRandom(hwndDlg);
      return TRUE;
    case IDC_BUTTON_FUN_ADD_WITH_NAME:
      onClickButtonFunAddWithName(hwndDlg);
      return TRUE;
    case IDC_BUTTON_FUN_DELETE_CURRENT_SELECTION:
      onClickButtonFunDeleteCurrentSelection(hwndDlg);
      return TRUE;
    case IDC_BUTTON_FUN_MODIFY_CURRENT_SELECTION:
      onClickButtonFunModifyCurrentSelection(hwndDlg);
      return TRUE;
    case IDC_BUTTON_FUN_QUERY_CURRENT_SELECTION:
      onClickButtonFunQueryCurrentSelection(hwndDlg);
      return TRUE;
    case IDC_BUTTON_FUN_QUERY_ID:
      onClickButtonFunQueryID(hwndDlg);
      return TRUE;
    case IDC_BUTTON_FUN_QUERY_NAME:
      onClickButtonFunQueryName(hwndDlg);
      return TRUE;
    case IDC_BUTTON_FUN_STUDENT_SELECTS_COURSE:
      onClickButtonFunStudentSelectsCourse(hwndDlg);
      return TRUE;
    case IDC_BUTTON_CONSOLE:
      onClickButtonConsole(hwndDlg);
      return TRUE;
    case IDC_BUTTON_EXIT:
      EndDialog(hwndDlg, lParam);
      return TRUE;
    }
  case WM_CLOSE:
    // EndDialog(hwndDlg, lParam);
    return TRUE;
  }
  
  return FALSE;
}

int onClickButtonConsole(HWND hwndDlg)
{
  char szConsoleInput[MAXBYTE] = {};
  GetDlgItemText(hwndDlg, IDC_EDIT_CONSOLE_INPUT, szConsoleInput,
    sizeof(szConsoleInput));

  CInformationManager* pInfoManager = GET_SINGLE(CInformationManager);
  pInfoManager->get_user_command((LPTSTR)szConsoleInput);
  if (pInfoManager->parser_user_command() == FALSE)
  {
    SetDlgItemText(hwndDlg, IDC_EDIT_CONSOLE_OUTPUT, INTERFACE_INPUT_ERROR_INFO);
    return TRUE;
  }

  if (pInfoManager->process_user_command() == FALSE)
  {
    SetDlgItemText(hwndDlg, IDC_EDIT_CONSOLE_OUTPUT, INTERFACE_PROCESS_ERROR_INFO);
  }
  else
  {
    SetDlgItemText(hwndDlg, IDC_EDIT_CONSOLE_OUTPUT, pInfoManager->get_output_buffer());
  }

  return 0;
}

int onInit(HWND hwndDlg)
{
  
  //初始化列表控件
  LVCOLUMN lvc;
  lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT; //需要用到fmt,cx, pszText三个成员
  lvc.fmt = LVCFMT_LEFT; //左对齐
  DWORD dwColIdx = 0;
  DWORD dwStyle = 0;
  
  // ==> 学生
  dwColIdx = 0;
  lvc.cx = 50; //宽度
  lvc.pszText = TEXT("ID");
  SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_INSERTCOLUMN, dwColIdx++, (LPARAM)&lvc);
  lvc.cx = 80;
  lvc.pszText = TEXT("姓名");
  SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_INSERTCOLUMN, dwColIdx++, (LPARAM)&lvc);
  lvc.cx = 205;
  lvc.pszText = TEXT("信息");
  SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_INSERTCOLUMN, dwColIdx++, (LPARAM)&lvc);

  dwStyle = (DWORD)SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT,
    LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
  dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
  SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_SETEXTENDEDLISTVIEWSTYLE, 
    0, dwStyle);

  // ==> 课程
  dwColIdx = 0;
  lvc.cx = 50;
  lvc.pszText = TEXT("ID");
  SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE, LVM_INSERTCOLUMN, dwColIdx++, (LPARAM)&lvc);
  lvc.cx = 273;
  lvc.pszText = TEXT("课程名称");
  SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE, LVM_INSERTCOLUMN, dwColIdx++, (LPARAM)&lvc);
  
  dwStyle = (DWORD)SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE,
    LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
  dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
  SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE, LVM_SETEXTENDEDLISTVIEWSTYLE,
    0, dwStyle);

  
  SetDlgItemText(hwndDlg, IDC_EDIT_STUDENT_LIST_PAGE_NUM, TEXT("1"));
  SetDlgItemText(hwndDlg, IDC_EDIT_COURSE_LIST_PAGE_NUM, TEXT("1"));

  SendDlgItemMessage(hwndDlg, IDC_BUTTON_STUDENT_LIST_FIRST_PAGE, BM_CLICK, 0, 0);
  SendDlgItemMessage(hwndDlg, IDC_BUTTON_COURSE_LIST_FIRST_PAGE, BM_CLICK, 0, 0);

  SendDlgItemMessage(hwndDlg, IDC_RADIO_STUDENT, BM_SETCHECK, BST_CHECKED, 0);

  return 0;
}

int SetPageNum(HWND hwndDlg, bool bDirection, int iListID, int iListMaxHeight, int iEditID)
{
  char szPageNumInput[MAXBYTE] = {};
  int iCurrentPage = -1;

  int iItemCount = SendDlgItemMessage(hwndDlg, iListID, LVM_GETITEMCOUNT, 0, 0);
  if (iItemCount >= 1)
  {
    LVITEM lviParam;
    lviParam.mask = LVIF_PARAM;
    lviParam.iItem = 0;
    ListView_GetItem(GetDlgItem(hwndDlg, iListID), &lviParam);

    iCurrentPage = (float)(lviParam.lParam - 0.0001) / iListMaxHeight + 2;
  }
  else
  {
    int iInc = bDirection ? 1 : -1;
    GetDlgItemText(hwndDlg, iEditID, szPageNumInput, sizeof(szPageNumInput));
    int iCurrentPageNumInput = atoi(szPageNumInput) ? atoi(szPageNumInput) : 1;
    iCurrentPage = iCurrentPageNumInput + iInc;
  }

  _stprintf(szPageNumInput, "%d", iCurrentPage);
  SetDlgItemText(hwndDlg, iEditID, szPageNumInput);

  return 0;
}

// 可以写成不定参，但需要查语法、改调用约定，比较麻烦
int InsertNewRow2List(HWND hwndDlg, int iListViewID, DWORD dwRowID, LPARAM lParam,
  int iColumnNum, LPTSTR szColumn1, LPTSTR szColumn2, LPTSTR szColumn3)
{
  DWORD dwRowIdx = dwRowID;
  LVITEM lvi;
  lvi.mask = LVIF_TEXT | LVIF_PARAM;
  lvi.iItem = dwRowIdx;
  lvi.iSubItem = 0;
  lvi.pszText = szColumn1;
  lvi.lParam = lParam;
  SendDlgItemMessage(hwndDlg, iListViewID, LVM_INSERTITEM, 0, (LPARAM)&lvi);

  DWORD dwSubItem = 1;

  if (iColumnNum > 1)
  {
    lvi.iItem = dwRowIdx;
    lvi.iSubItem = dwSubItem++;
    lvi.pszText = szColumn2;
    SendDlgItemMessage(hwndDlg, iListViewID, LVM_SETITEMTEXT, dwRowIdx, (LPARAM)&lvi);
  }
  
  if (iColumnNum > 2)
  {
    lvi.iItem = dwRowIdx;
    lvi.iSubItem = dwSubItem++;
    lvi.pszText = szColumn3;
    SendDlgItemMessage(hwndDlg, iListViewID, LVM_SETITEMTEXT, dwRowIdx, (LPARAM)&lvi);
  }

  return 0;
}

int SerialAddEntries2List(HWND hwndDlg, int iListControl, bool bDirection, 
  int iExpectedEntryNum, LPCTSTR szCmd, int iStartingID, 
  int iMaxID, int iMinID)
{
  CInformationManager* pInfoManager = GET_SINGLE(CInformationManager);

  TCHAR szBufferCmd[MAXBYTE] = { 0 };
  TCHAR szBufferOutput[MAXBYTE * 16] = { 0 };

  CGYDynamicArray<CGYString*> daryDataBuffer;
 
  int iInc = bDirection ? 1 : -1;

  for (int i = 0; i < iExpectedEntryNum; i++)
  {
    if (iStartingID < iMinID || iStartingID > iMaxID)
    {
      break;
    }

    // craft the cmd & execute
    _stprintf(szBufferCmd, szCmd, iStartingID);
    pInfoManager->get_user_command((LPTSTR)szBufferCmd);
    pInfoManager->parser_user_command();
    pInfoManager->process_user_command();
    _stprintf(szBufferOutput, "%s", pInfoManager->get_output_buffer());

    daryDataBuffer.empty_array();
    CGYString::split_string_into_dynamic_array(szBufferOutput, daryDataBuffer, '\n');

    // "没有符合查询条件的结果"
    if (daryDataBuffer.get_array_element_count() <= 2)
    {
      i--;
      iStartingID += iInc;
      continue;
    }

    InsertNewRow2List(hwndDlg, iListControl, bDirection ? i : 0,
      (LPARAM)atoi(**(daryDataBuffer[0]) + 4),
      iListControl == IDC_LIST_STUDENT? 3 : 2,
      (LPTSTR)(**(daryDataBuffer[0]) + 4),  //+4: "ID:[s]" removed
      (LPTSTR)(**(daryDataBuffer[1]) + 6),
      (LPTSTR)(**(daryDataBuffer[2]) + 6));

    iStartingID += iInc;
  }

  return 0;
}

int onClickButtonStudentListFirstPage(HWND hwndDlg)
{
  SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_DELETEALLITEMS, 0, 0);

  SerialAddEntries2List(hwndDlg, IDC_LIST_STUDENT, TRUE, 
    STUDENT_LIST_HEIGHT, TEXT("-l %d"), 0, 
    CStudent::nIDSerialCounter - 1);

  SetPageNum(hwndDlg, FALSE, IDC_LIST_STUDENT, STUDENT_LIST_HEIGHT,
    IDC_EDIT_STUDENT_LIST_PAGE_NUM);

  return 0;
}

int onClickButtonStudentListPreviousPage(HWND hwndDlg)
{
  int iStartingID = CStudent::nIDSerialCounter - 1;

  int iItemCount = SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_GETITEMCOUNT, 0, 0);
  if (iItemCount >= 1)
  {
    LVITEM lviParam;
    lviParam.mask = LVIF_PARAM;
    lviParam.iItem = 0;
    ListView_GetItem(GetDlgItem(hwndDlg, IDC_LIST_STUDENT), &lviParam);
    iStartingID = lviParam.lParam - 1;
  }

  SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_DELETEALLITEMS, 0, 0);

  SerialAddEntries2List(hwndDlg, IDC_LIST_STUDENT, FALSE,
    STUDENT_LIST_HEIGHT, TEXT("-l %d"), iStartingID,
    CStudent::nIDSerialCounter - 1);

  SetPageNum(hwndDlg, FALSE, IDC_LIST_STUDENT, STUDENT_LIST_HEIGHT,
    IDC_EDIT_STUDENT_LIST_PAGE_NUM);

  return 0;
}

int onClickButtonStudentListGoPage(HWND hwndDlg)
{
  char szPageNumInput[MAXBYTE] = {};
  GetDlgItemText(hwndDlg, IDC_EDIT_STUDENT_LIST_PAGE_NUM, szPageNumInput,
    sizeof(szPageNumInput));

  int iPageNum = atoi(szPageNumInput) ? atoi(szPageNumInput) : 1;
  iPageNum = iPageNum < 0 ? 1 : iPageNum;

  _stprintf(szPageNumInput, "%d", iPageNum);
  SetDlgItemText(hwndDlg, IDC_EDIT_STUDENT_LIST_PAGE_NUM, szPageNumInput);

  int iStartingID = (iPageNum - 1) * STUDENT_LIST_HEIGHT;

  SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_DELETEALLITEMS, 0, 0);

  SerialAddEntries2List(hwndDlg, IDC_LIST_STUDENT, TRUE,
    STUDENT_LIST_HEIGHT, TEXT("-l %d"), iStartingID,
    CStudent::nIDSerialCounter - 1);

  return 0;
}

int onClickButtonStudentListNextPage(HWND hwndDlg)
{
  int iStartingID = 0;

  int iItemCount = SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_GETITEMCOUNT, 0, 0);
  if (iItemCount >= 1)
  {
    LVITEM lviParam;
    lviParam.mask = LVIF_PARAM;
    lviParam.iItem = iItemCount - 1;
    ListView_GetItem(GetDlgItem(hwndDlg, IDC_LIST_STUDENT), &lviParam);
    iStartingID = lviParam.lParam + 1;
  }
  
  SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_DELETEALLITEMS, 0, 0);

  SerialAddEntries2List(hwndDlg, IDC_LIST_STUDENT, TRUE,
    STUDENT_LIST_HEIGHT, TEXT("-l %d"), iStartingID,
    CStudent::nIDSerialCounter - 1);

  SetPageNum(hwndDlg, TRUE, IDC_LIST_STUDENT, STUDENT_LIST_HEIGHT, 
    IDC_EDIT_STUDENT_LIST_PAGE_NUM);

  return 0;
}

int onClickButtonStudentListLastPage(HWND hwndDlg)
{
  SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_DELETEALLITEMS, 0, 0);

  SerialAddEntries2List(hwndDlg, IDC_LIST_STUDENT, FALSE,
    STUDENT_LIST_HEIGHT, TEXT("-l %d"), CStudent::nIDSerialCounter - 1,
    CStudent::nIDSerialCounter - 1);

  SetPageNum(hwndDlg, TRUE, IDC_LIST_STUDENT, STUDENT_LIST_HEIGHT,
    IDC_EDIT_STUDENT_LIST_PAGE_NUM);

  return 0;
}

int onClickButtonCourseListFirstPage(HWND hwndDlg)
{
  SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE, LVM_DELETEALLITEMS, 0, 0);

  SerialAddEntries2List(hwndDlg, IDC_LIST_COURSE, TRUE,
    COURSE_LIST_HEIGHT, TEXT("-L %d"), 0,
    CCourse::nIDSerialCounter - 1);

  SetPageNum(hwndDlg, FALSE, IDC_LIST_COURSE, COURSE_LIST_HEIGHT,
    IDC_EDIT_COURSE_LIST_PAGE_NUM);

  return 0;
}

int onClickButtonCourseListPreviousPage(HWND hwndDlg)
{
  int iStartingID = CCourse::nIDSerialCounter - 1;

  int iItemCount = SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE, LVM_GETITEMCOUNT, 0, 0);
  if (iItemCount >= 1)
  {
    LVITEM lviParam;
    lviParam.mask = LVIF_PARAM;
    lviParam.iItem = 0;
    ListView_GetItem(GetDlgItem(hwndDlg, IDC_LIST_COURSE), &lviParam);
    iStartingID = lviParam.lParam - 1;
  }

  SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE, LVM_DELETEALLITEMS, 0, 0);

  SerialAddEntries2List(hwndDlg, IDC_LIST_COURSE, FALSE,
    COURSE_LIST_HEIGHT, TEXT("-L %d"), iStartingID,
    CCourse::nIDSerialCounter - 1);

  SetPageNum(hwndDlg, FALSE, IDC_LIST_COURSE, COURSE_LIST_HEIGHT,
    IDC_EDIT_COURSE_LIST_PAGE_NUM);

  return 0;
}

int onClickButtonCourseListGoPage(HWND hwndDlg)
{
  char szPageNumInput[MAXBYTE] = {};
  GetDlgItemText(hwndDlg, IDC_EDIT_COURSE_LIST_PAGE_NUM, szPageNumInput,
    sizeof(szPageNumInput));

  int iPageNum = atoi(szPageNumInput) ? atoi(szPageNumInput) : 1;
  iPageNum = iPageNum < 0 ? 1 : iPageNum;

  _stprintf(szPageNumInput, "%d", iPageNum);
  SetDlgItemText(hwndDlg, IDC_EDIT_COURSE_LIST_PAGE_NUM, szPageNumInput);

  int iStartingID = (iPageNum - 1) * COURSE_LIST_HEIGHT;

  SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE, LVM_DELETEALLITEMS, 0, 0);

  SerialAddEntries2List(hwndDlg, IDC_LIST_COURSE, TRUE,
    COURSE_LIST_HEIGHT, TEXT("-L %d"), iStartingID,
    CCourse::nIDSerialCounter - 1);

  return 0;
}

int onClickButtonCourseListNextPage(HWND hwndDlg)
{
  int iStartingID = 0;

  int iItemCount = SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE, LVM_GETITEMCOUNT, 0, 0);
  if (iItemCount >= 1)
  {
    LVITEM lviParam;
    lviParam.mask = LVIF_PARAM;
    lviParam.iItem = iItemCount - 1;
    ListView_GetItem(GetDlgItem(hwndDlg, IDC_LIST_COURSE), &lviParam);
    iStartingID = lviParam.lParam + 1;
  }

  SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE, LVM_DELETEALLITEMS, 0, 0);

  SerialAddEntries2List(hwndDlg, IDC_LIST_COURSE, TRUE,
    COURSE_LIST_HEIGHT, TEXT("-L %d"), iStartingID,
    CCourse::nIDSerialCounter - 1);

  SetPageNum(hwndDlg, TRUE, IDC_LIST_COURSE, COURSE_LIST_HEIGHT,
    IDC_EDIT_COURSE_LIST_PAGE_NUM);

  return 0;
}

int onClickButtonCourseListLastPage(HWND hwndDlg)
{
  SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE, LVM_DELETEALLITEMS, 0, 0);

  SerialAddEntries2List(hwndDlg, IDC_LIST_COURSE, FALSE,
    COURSE_LIST_HEIGHT, TEXT("-L %d"), CCourse::nIDSerialCounter - 1,
    CCourse::nIDSerialCounter - 1);

  SetPageNum(hwndDlg, TRUE, IDC_LIST_COURSE, COURSE_LIST_HEIGHT,
    IDC_EDIT_COURSE_LIST_PAGE_NUM);

  return 0;
}

CGYString CommandExecute(LPTSTR szCmd)
{
  CInformationManager* pInfoManager = GET_SINGLE(CInformationManager);

  pInfoManager->get_user_command(szCmd);
  pInfoManager->parser_user_command();
  pInfoManager->process_user_command();

  CGYString szOutput(pInfoManager->get_output_buffer());

  return szOutput;
}

int onClickButtonFunAddRandom(HWND hwndDlg)
{
  if (SendDlgItemMessage(hwndDlg, IDC_RADIO_STUDENT, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    CommandExecute(TEXT("-g 5"));
    SendDlgItemMessage(hwndDlg, IDC_BUTTON_STUDENT_LIST_LAST_PAGE, BM_CLICK, 0, 0);
  }

  if (SendDlgItemMessage(hwndDlg, IDC_RADIO_COURSE, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    CommandExecute(TEXT("-G 5"));
    SendDlgItemMessage(hwndDlg, IDC_BUTTON_COURSE_LIST_LAST_PAGE, BM_CLICK, 0, 0);
  }
  
  return 0;
}

int onClickButtonFunAddWithName(HWND hwndDlg)
{
  char szCmd[MAXBYTE] = {};

  char szNameInput[MAXBYTE] = {};
  GetDlgItemText(hwndDlg, IDC_EDIT_FUN_ADD_WITH_NAME, szNameInput, sizeof(szNameInput));

  if (SendDlgItemMessage(hwndDlg, IDC_RADIO_STUDENT, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    _stprintf(szCmd, "-a \"%s\"", szNameInput);
    CommandExecute(szCmd);
    SendDlgItemMessage(hwndDlg, IDC_BUTTON_STUDENT_LIST_LAST_PAGE, BM_CLICK, 0, 0);
  }

  if (SendDlgItemMessage(hwndDlg, IDC_RADIO_COURSE, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    _stprintf(szCmd, "-A \"%s\"", szNameInput);
    CommandExecute(szCmd);
    SendDlgItemMessage(hwndDlg, IDC_BUTTON_COURSE_LIST_LAST_PAGE, BM_CLICK, 0, 0);
  }

  return 0;
}

int onClickButtonFunDeleteCurrentSelection(HWND hwndDlg)
{
  char szCmd[MAXBYTE] = {};

  LVITEM lviParam;
  lviParam.mask = LVIF_PARAM;

  if (SendDlgItemMessage(hwndDlg, IDC_RADIO_STUDENT, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    DWORD dwSelIdx = SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_GETSELECTIONMARK, 0, 0);
    if (dwSelIdx == -1)
    {
      MessageBox(hwndDlg, TEXT("没有选中学生"), TEXT("ERROR"), 0);
      return 0;
    }

    lviParam.iItem = dwSelIdx;
    ListView_GetItem(GetDlgItem(hwndDlg, IDC_LIST_STUDENT), &lviParam);
    int iID = lviParam.lParam;

    _stprintf(szCmd, "-d %d", iID);
    CGYString szOutput = CommandExecute(szCmd);
    MessageBox(hwndDlg, *szOutput, TEXT("删除"), 0);

    SendDlgItemMessage(hwndDlg, IDC_BUTTON_STUDENT_LIST_GO_PAGE, BM_CLICK, 0, 0);
  }

  if (SendDlgItemMessage(hwndDlg, IDC_RADIO_COURSE, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    DWORD dwSelIdx = SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE, LVM_GETSELECTIONMARK, 0, 0);
    if (dwSelIdx == -1)
    {
      MessageBox(hwndDlg, TEXT("没有选中课程"), TEXT("ERROR"), 0);
      return 0;
    }

    lviParam.iItem = dwSelIdx;
    ListView_GetItem(GetDlgItem(hwndDlg, IDC_LIST_COURSE), &lviParam);
    int iID = lviParam.lParam;

    _stprintf(szCmd, "-D %d", iID);
    CGYString szOutput = CommandExecute(szCmd);
    MessageBox(hwndDlg, *szOutput, TEXT("删除"), 0);

    SendDlgItemMessage(hwndDlg, IDC_BUTTON_COURSE_LIST_GO_PAGE, BM_CLICK, 0, 0);
  }

  return 0;
}

int onClickButtonFunModifyCurrentSelection(HWND hwndDlg)
{
  char szCmd[MAXBYTE] = {};

  char szNameInput[MAXBYTE] = {};
  GetDlgItemText(hwndDlg, IDC_EDIT_FUN_MODIFY_CURRENT_SELECTION, szNameInput, sizeof(szNameInput));

  if (strlen(szNameInput) <= 0)
  {
    MessageBox(hwndDlg, TEXT("没有输入名字"), TEXT("ERROR"), 0);
    return 0;
  }

  LVITEM lviParam;
  lviParam.mask = LVIF_PARAM;

  if (SendDlgItemMessage(hwndDlg, IDC_RADIO_STUDENT, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    DWORD dwSelIdx = SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_GETSELECTIONMARK, 0, 0);
    if (dwSelIdx == -1)
    {
      MessageBox(hwndDlg, TEXT("没有选中学生"), TEXT("ERROR"), 0);
      return 0;
    }

    lviParam.iItem = dwSelIdx;
    ListView_GetItem(GetDlgItem(hwndDlg, IDC_LIST_STUDENT), &lviParam);
    int iID = lviParam.lParam;

    _stprintf(szCmd, "-m %d \"%s\"", iID, szNameInput);
    CommandExecute(szCmd);
    
    SendDlgItemMessage(hwndDlg, IDC_BUTTON_STUDENT_LIST_GO_PAGE, BM_CLICK, 0, 0);
  }

  if (SendDlgItemMessage(hwndDlg, IDC_RADIO_COURSE, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    DWORD dwSelIdx = SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE, LVM_GETSELECTIONMARK, 0, 0);
    if (dwSelIdx == -1)
    {
      MessageBox(hwndDlg, TEXT("没有选中课程"), TEXT("ERROR"), 0);
      return 0;
    }

    lviParam.iItem = dwSelIdx;
    ListView_GetItem(GetDlgItem(hwndDlg, IDC_LIST_COURSE), &lviParam);
    int iID = lviParam.lParam;

    _stprintf(szCmd, "-M %d \"%s\"", iID, szNameInput);
    CommandExecute(szCmd);

    SendDlgItemMessage(hwndDlg, IDC_BUTTON_COURSE_LIST_GO_PAGE, BM_CLICK, 0, 0);
  }

  return 0;
}


int onClickButtonFunQueryCurrentSelection(HWND hwndDlg)
{
  char szCmd[MAXBYTE] = {};

  LVITEM lviParam;
  lviParam.mask = LVIF_PARAM;

  if (SendDlgItemMessage(hwndDlg, IDC_RADIO_STUDENT, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    DWORD dwSelIdx = SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_GETSELECTIONMARK, 0, 0);
    if (dwSelIdx == -1)
    {
      MessageBox(hwndDlg, TEXT("没有选中学生"), TEXT("ERROR"), 0);
      return 0;
    }

    lviParam.iItem = dwSelIdx;
    ListView_GetItem(GetDlgItem(hwndDlg, IDC_LIST_STUDENT), &lviParam);
    int iID = lviParam.lParam;

    _stprintf(szCmd, "-l %d", iID);
  }

  if (SendDlgItemMessage(hwndDlg, IDC_RADIO_COURSE, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    DWORD dwSelIdx = SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE, LVM_GETSELECTIONMARK, 0, 0);
    if (dwSelIdx == -1)
    {
      MessageBox(hwndDlg, TEXT("没有选中课程"), TEXT("ERROR"), 0);
      return 0;
    }

    lviParam.iItem = dwSelIdx;
    ListView_GetItem(GetDlgItem(hwndDlg, IDC_LIST_COURSE), &lviParam);
    int iID = lviParam.lParam;

    _stprintf(szCmd, "-L %d", iID);
  }

  CGYString szOutput = CommandExecute((LPTSTR)szCmd);
  MessageBox(hwndDlg, *szOutput, TEXT("查询结果"), 0);

  return 0;
}

int onClickButtonFunQueryID(HWND hwndDlg)
{
  char szCmd[MAXBYTE] = {};

  char szIDInput[MAXBYTE] = {};
  GetDlgItemText(hwndDlg, IDC_EDIT_FUN_QUERY_ID, szIDInput, sizeof(szIDInput));

  int iID = atoi(szIDInput);  // 控件输入限制为仅数字，无需检查
  

  if (SendDlgItemMessage(hwndDlg, IDC_RADIO_STUDENT, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    _stprintf(szCmd, "-l %d", iID);
  }

  if (SendDlgItemMessage(hwndDlg, IDC_RADIO_COURSE, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    _stprintf(szCmd, "-L %d", iID);
  }

  CGYString szOutput = CommandExecute((LPTSTR)szCmd);
  MessageBox(hwndDlg, *szOutput, TEXT("查询结果"), 0);

  return 0;
}

int onClickButtonFunQueryName(HWND hwndDlg)
{
  char szCmd[MAXBYTE] = {};

  char szNameInput[MAXBYTE] = {};
  GetDlgItemText(hwndDlg, IDC_EDIT_FUN_QUERY_NAME, szNameInput, sizeof(szNameInput));

  if (strlen(szNameInput) <= 0)
  {
    MessageBox(hwndDlg, TEXT("没有输入名字"), TEXT("ERROR"), 0);
    return 0;
  }

  if (SendDlgItemMessage(hwndDlg, IDC_RADIO_STUDENT, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    _stprintf(szCmd, "-l \"%s\"", szNameInput);
  }

  if (SendDlgItemMessage(hwndDlg, IDC_RADIO_COURSE, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    _stprintf(szCmd, "-L \"%s\"", szNameInput);
  }

  CGYString szOutput = CommandExecute((LPTSTR)szCmd);
  MessageBox(hwndDlg, *szOutput, TEXT("查询结果"), 0);

  return 0;
}

int onClickButtonFunStudentSelectsCourse(HWND hwndDlg)
{
  char szCmd[MAXBYTE] = {};

  LVITEM lviParam;
  lviParam.mask = LVIF_PARAM;

  DWORD dwSelIdx = SendDlgItemMessage(hwndDlg, IDC_LIST_STUDENT, LVM_GETSELECTIONMARK, 0, 0);
  if (dwSelIdx == -1)
  {
    MessageBox(hwndDlg, TEXT("没有选中学生"), TEXT("ERROR"), 0);
    return 0;
  }

  lviParam.iItem = dwSelIdx;
  ListView_GetItem(GetDlgItem(hwndDlg, IDC_LIST_STUDENT), &lviParam);
  int iStudentID = lviParam.lParam;

  dwSelIdx = SendDlgItemMessage(hwndDlg, IDC_LIST_COURSE, LVM_GETSELECTIONMARK, 0, 0);
  if (dwSelIdx == -1)
  {
    MessageBox(hwndDlg, TEXT("没有选中课程"), TEXT("ERROR"), 0);
    return 0;
  }

  lviParam.iItem = dwSelIdx;
  ListView_GetItem(GetDlgItem(hwndDlg, IDC_LIST_COURSE), &lviParam);
  int iCourseID = lviParam.lParam;

  _stprintf(szCmd, "-c %d %d", iStudentID, iCourseID);

  CGYString szOutput = CommandExecute((LPTSTR)szCmd);
  MessageBox(hwndDlg, *szOutput, TEXT("选课结果"), 0);

  return 0;
}