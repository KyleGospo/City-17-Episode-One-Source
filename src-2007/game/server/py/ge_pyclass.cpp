///////////// Copyright © 2009 LodleNet. All rights reserved. /////////////
//
//   Project     : Server
//   File        : ge_pyclass.cpp
//   Description :
//      [TODO: Write the purpose of ge_pyclass.cpp.]
//
//   Created On: 8/24/2009 3:39:13 PM
//   Created By: Mark Chandler <mailto:mark@moddb.com>
////////////////////////////////////////////////////////////////////////////

#include "cbase.h"
#include "ge_pymanager.h"

#include <boost/python.hpp>
namespace bp = boost::python;
 

 
pythExample::pythExample()
{
}
 
void pythExample::setMsg(const char* msg)
{
	TRYFUNC( this->get_override("setMsg")(msg));
}
 
virtual const char* pythExample::getMsg()
{
	TRYFUNCRET( this->get_override("getMsg")(), "NULL");
}
 
void pythExample::sayHallo()
{
	TRYFUNC( this->get_override("sayHallo")());
}

BOOST_PYTHON_MODULE(HLExample)
{
	 bp::class_<pythExample>("pythExample")
		.def("setMsg", &pythExample::setMsg)
		.def("getMsg", &pythExample::getMsg)
		.def("sayHallo", &pythExample::sayHallo);
}