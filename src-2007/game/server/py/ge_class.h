///////////// Copyright © 2009 LodleNet. All rights reserved. /////////////
//
//   Project     : Server
//   File        : ge_pyclass.h
//   Description :
//      [TODO: Write the purpose of ge_pytest.h.]
//
//   Created On: 9/1/2009 9:42:38 PM
//   Created By: Mark Chandler <mailto:mark@moddb.com>
////////////////////////////////////////////////////////////////////////////

#ifndef MC_GE_PYCLASS_H
#define MC_GE_PYCLASS_H
#ifdef _WIN32
#pragma once
#endif

class pythExampleI
{
public:
	const char* getMsg()=0;
	void setMsg(const char*)=0;
	void sayHallo()=0;
};

#endif