///////////// Copyright © 2009 LodleNet. All rights reserved. /////////////
//
//   Project     : Server
//   File        : ge_pyclass.h
//   Description :
//      [TODO: Write the purpose of ge_pyclass.h.]
//
//   Created On: 9/1/2009 9:42:38 PM
//   Created By: Mark Chandler <mailto:mark@moddb.com>
////////////////////////////////////////////////////////////////////////////

#ifndef MC_GE_PYCLASS_H
#define MC_GE_PYCLASS_H
#ifdef _WIN32
#pragma once
#endif

#include "ge_class.h"
#include <boost/python.hpp>
namespace bp = boost::python;

class pythExample : public pyExampleI, public bp::wrapper<pythExample>
{
public:
	pythExample();
 
	void setMsg(const char* msg);
	const char* getMsg();
	void sayHallo();
}


#endif