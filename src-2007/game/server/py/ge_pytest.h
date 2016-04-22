///////////// Copyright © 2009 LodleNet. All rights reserved. /////////////
//
//   Project     : Server
//   File        : ge_pytest.h
//   Description :
//      [TODO: Write the purpose of ge_pytest.h.]
//
//   Created On: 9/1/2009 9:42:38 PM
//   Created By: Mark Chandler <mailto:mark@moddb.com>
////////////////////////////////////////////////////////////////////////////

#ifndef MC_GE_PYTEST_H
#define MC_GE_PYTEST_H
#ifdef _WIN32
#pragma once
#endif

#include <boost/python.hpp>
#include <vector>
namespace bp = boost::python;

#include "ge_pymanager.h"



class CMYPyHandle : public PyHandle
{
public:
	CMYPyHandle();

	void Init();
	void Shutdown();
};

extern CMYPyHandle* GetPyHandle();

#endif
