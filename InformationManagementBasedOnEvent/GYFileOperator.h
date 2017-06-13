#pragma once

#include "GYString.h"

typedef long charnumber_t;

//文件操作类，暂时没写检查（new，fopen的空指针检查）
class CGYFileOperator
{
public:
  //CGYFileOperator();
  CGYFileOperator(const char * pszFileName);
  virtual ~CGYFileOperator();

public:
  //返回文件的大小（字节数）
  charnumber_t get_file_size_byte();
  //文件映射到内存
  int map_file_to_memory(char ** pszMemDest, charnumber_t nStartPoint = 0,
    charnumber_t nCharNum = -1, bool isEndWith0 = true);
  //向定位置写入内容，如果不设定位置，则向尾部插入
  bool write_string_into_file(const char * pszContent, charnumber_t nOffset = -1);

private:
  CGYString m_StrFileName;
  FILE *m_pFile;
  //如果文件不存在（第一次运行时），建立文件
  int create_file_in_disk();
};

