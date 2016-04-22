///////////// Copyright © 2009 LodleNet. All rights reserved. /////////////
//
//   Project     : Server
//   File        : ge_pyexample.cpp
//   Description :
//      [TODO: Write the purpose of ge_pyexample.cpp.]
//
//   Created On: 8/24/2009 3:39:13 PM
//   Created By: Mark Chandler <mailto:mark@moddb.com>
////////////////////////////////////////////////////////////////////////////

#include "cbase.h"
#include "ge_pyexample.h"

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
	
	m_poPYClass = NULL;
}

void CMYPyHandle::Init()
{
	try
	{
		ExecFile("example.py");
		m_poPYClass = bp::extract<pythExample*>(GetDict()["NewPythonClass"]());
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
