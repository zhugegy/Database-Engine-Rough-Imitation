// InformationManagementBasedOnEvent.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GYSingleInstanceDedicatedClass.h"
#include "InformationManager.h"
#include "Student.h"
#include <time.h>
#include <stdlib.h>
#include "Course.h"
#include "EventFYJC.h"
#include <stdio.h>

CGYSingleInstanceDedicatedClass do_not_use_created_only_for_single_instance;


int main()
{
  srand((unsigned int)time(0));

  CInformationManager *pInfoManager = GET_SINGLE(CInformationManager)
  pInfoManager->load_info();
  pInfoManager->load_static_serials();
  pInfoManager->main_loop();

  return 0;
}

/*全清空再全部写入的方法，留做备份。*/
/*
/* FILE *pTmp = fopen("data_files\\course\\roster\\roster.txt", "w");
fclose(pTmp);

pTmp = fopen("data_files\\course\\data\\data.txt", "w");
fclose(pTmp);

pTmp = fopen("data_files\\student\\roster\\roster.txt", "w");
fclose(pTmp);

pTmp = fopen("data_files\\student\\data\\data.txt", "w");
fclose(pTmp);

pTmp = fopen("data_files\\eventFYJC\\roster\\roster.txt", "w");
fclose(pTmp);

pTmp = fopen("data_files\\eventFYJC\\data\\data.txt", "w");
fclose(pTmp);

for (CGYLinkList<CGeneralizedObject *>::iterator it = CGeneralizedObject::pAllObjects.begin();
it != CGeneralizedObject::pAllObjects.end();
it++)
{
if ((*it)->get_name() != NULL)
{
(*it)->when_being_added_an_event();
}
}*/