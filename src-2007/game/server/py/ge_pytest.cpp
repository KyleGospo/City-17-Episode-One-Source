///////////// Copyright © 2009 LodleNet. All rights reserved. /////////////
//
//   Project     : Server
//   File        : ge_pygameplay.cpp
//   Description :
//      [TODO: Write the purpose of ge_pygameplay.cpp.]
//
//   Created On: 8/24/2009 3:39:13 PM
//   Created By: Mark Chandler <mailto:mark@moddb.com>
////////////////////////////////////////////////////////////////////////////

#include "cbase.h"
#include "ge_pytest.h"

CMYPyHandle *g_PyHandle = NULL;
CMYPyHandle *GetPyHandle()
{
	return g_PyHandle;
}


CMYPyHandle::CMYPyHandle()
{
	g_MyHandle = this;
	//make sure we call this!
	Register();
}

void CMYPyHandle::Init()
{
	try
	{
		ExecFile("test.py");
	}
	catch (...)
	{
		HandlePythonException();
	}
}

void CMYPyHandle::Shutdown()
{
}

pyExampleI* CMYPyHandle::GetMyClass()
{
	return dynamic_cast<pyExampleI*>();
}

#include "ge_pytest.h"

CONCOMMAND(python_test, "Test out python!")
{
	pyExampleI* pyClass = GetPyHandle()->GetMyClass();
	
	pyClass->sayHallo();
	
	Msg(pyClass->getMsg());
	Msg("\n");
	
	pyClass->setMsg("This is a test Message!");
	
	Msg(pyClass->getMsg());
	Msg("\n");
}
