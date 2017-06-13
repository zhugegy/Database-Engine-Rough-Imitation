#include "stdafx.h"
#include "GYSingleInstanceDedicatedClass.h"
#include "GYObjFactory.h"
#include "GYString.h"
#include "Student.h"
#include "InformationManager.h"
#include "Course.h"
#include "EventFYJC.h"

CGYSingleInstanceDedicatedClass::CGYSingleInstanceDedicatedClass()
{
	CREATE_SINGLE(CGYObjFactory<CGYString>)
  CREATE_SINGLE(CGYObjFactory<CStudent>)
  CREATE_SINGLE(CGYObjFactory<CCourse>)
  CREATE_SINGLE(CGYObjFactory<CEventFYJC>)
  CREATE_SINGLE(CInformationManager)

}


CGYSingleInstanceDedicatedClass::~CGYSingleInstanceDedicatedClass()
{
	RELEASE_SINGLE(CGYObjFactory<CGYString>)
  RELEASE_SINGLE(CGYObjFactory<CStudent>)
  RELEASE_SINGLE(CGYObjFactory<CCourse>)
  RELEASE_SINGLE(CGYObjFactory<CEventFYJC>)
  RELEASE_SINGLE(CInformationManager)

}
