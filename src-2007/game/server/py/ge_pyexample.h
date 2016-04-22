///////////// Copyright © 2009 LodleNet. All rights reserved. /////////////
//
//   Project     : Server
//   File        : ge_pyexample.h
//   Description :
//      [TODO: Write the purpose of ge_pytest.h.]
//
//   Created On: 9/1/2009 9:42:38 PM
//   Created By: Mark Chandler <mailto:mark@moddb.com>
////////////////////////////////////////////////////////////////////////////

#ifndef MC_GE_PYEXAMPLE_H
#define MC_GE_PYEXAMPLE_H
#ifdef _WIN32
#pragma once
#endif

#include <boost/python.hpp>
#include <vector>
namespace bp = boost::python;

#include "ge_pymanager.h"
#include "ge_pyclass.h"


class CMYPyHandle : public PyHandle
{
public:
	CMYPyHandle();

	void Init();
	void Shutdown();

	pyExampleI* GetMyClass();
 
private:
	pythExample* m_poPYClass;	
};

extern CMYPyHandle* GetPyHandle();

#endif
