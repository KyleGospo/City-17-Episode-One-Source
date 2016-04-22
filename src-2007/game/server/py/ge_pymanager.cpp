///////////// Copyright © 2009 LodleNet. All rights reserved. /////////////
//
//   Project     : Server
//   File        : ge_pymanager.cpp
//   Description :
//      [TODO: Write the purpose of ge_pymanager.cpp.]
//
//   Created On: 8/31/2009 9:07:05 PM
//   Created By: Mark Chandler <mailto:mark@moddb.com>
////////////////////////////////////////////////////////////////////////////

#include "cbase.h"
#include "filesystem.h"
#include "fmtstr.h"

#include <boost/python.hpp>
namespace bp = boost::python;

#include "ge_pymanager.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern "C" void RegisterPythonModules();

namespace boost
{
void throw_exception(std::exception const & e)
{
    throw e;
} 
}


CGEPYManager gPyMng;
CGEPYManager* GEPython()
{
	return &gPyMng;
}

void PythonInit()
{
	gPyMng.InitDll();
}

void PythonShutdown()
{
	gPyMng.ShutdownDll();
}

void PythonInitHandles()
{
	gPyMng.InitHandles();
}

void PythonShutdownHandles()
{
	gPyMng.ShutdownHandles();
}

////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////

PyHandle::PyHandle()
{
	m_bStarted = false;
}

PyHandle::~PyHandle()
{
	GEPython()->DeRegisterHandle(this);
}

void PyHandle::Register()
{
	GEPython()->RegisterHandle(this);
}

void PyHandle::InitHandle()
{
	if ( m_bStarted )
		return;

	m_bStarted = true;

	m_oDict = bp::object(bp::handle<>( PyDict_Copy( GEPython()->GetGloablDict().ptr() )));
	Init();
}

void PyHandle::ShutdownHandle()
{
	if ( !m_bStarted )
		return;

	Shutdown();
	delete this;
}

bp::object PyHandle::Exec(const char* buff)
{
	return bp::exec(buff, GetDict(), GetDict());
}

bp::object PyHandle::ExecFile(const char* file)
{
	char pf[255];

	Q_snprintf(pf, 255, "%s\\%s", GEPython()->GetRootPath(), file);

	char fullPath[255];
	filesystem->RelativePathToFullPath( pf, "MOD", fullPath, 255);

	return bp::exec_file(fullPath, GetDict(), GetDict());
}

////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////


CGEPYManager::CGEPYManager()
{
	m_bInit = false;
	m_bHandleInit = false;
}

CGEPYManager::~CGEPYManager()
{

}

void CGEPYManager::InitDll()
{
	if ( m_bInit )
		return;

	try
	{
		char filePath[255];
		filesystem->RelativePathToFullPath(GetRootPath(), "MOD", filePath, sizeof(filePath) );
		Py_SetPythonHome(filePath);

		RegisterPythonModules();
		Py_Initialize();

		main_module = bp::import("__main__");
		main_namespace = main_module.attr("__dict__");

		main_module.attr("true") = true;
		main_module.attr("false") = false;

		try
		{
			ExecFile("IORedirect.py");
		}
		catch (...)
		{
			HandlePythonException();
			Warning("Failed to load IORedirect!\n");
		}

#if defined (WINGDBG) && (DEBUG)
		try
		{
			Exec("import wingdbstub");
		}
		catch (...)
		{
			Warning("Failed to import python debugger\n");
			HandlePythonException();
		}
#endif

		CFmtStr globalpath;
		globalpath.sprintf( "C17PyBaseDir = \"%s\"", filePath );
		try
		{
			Exec(globalpath.Access());
		}
		catch (...)
		{
			Warning("Failed to setup python base path global!\n");
			HandlePythonException();
		}

		try
		{
			ExecFile("C17Init.py");
		}
		catch (...)
		{
			HandlePythonException();
			Warning("Failed to load python initialization script!\n");
		}

		m_bInit = true;
	}
	catch (...)
	{
		HandlePythonException();
		Warning("Failed to load python!\n");
	}
}

void CGEPYManager::ShutdownDll()
{
	if ( !m_bInit )
		return;

	ShutdownHandles();

	m_bInit = false;

	//Py_Finalize();
}

void CGEPYManager::InitHandles( void )
{
	if ( !m_bInit || m_bHandleInit )
	{
		Assert(false);
		return;
	}

	for (int x=0; x<m_vHandles.Size(); x++)
	{
		m_vHandles[x]->InitHandle();
	}

	m_bHandleInit = true;
}

void CGEPYManager::ShutdownHandles( void )
{
	//hnadles will removet them selfs as we go along the list
	for (int x=m_vHandles.Size()-1; x >= 0; x--)
	{
		m_vHandles[x]->ShutdownHandle();
	}

	m_bHandleInit = false;
}

void CGEPYManager::DeRegisterHandle(PyHandle* handle)
{
	if (!handle)
		return;
	
	for (int x=0; x<m_vHandles.Size(); x++)
	{
		if (m_vHandles[x]==handle)
		{
			m_vHandles.Remove(x);
			break;
		}
	}
}

void CGEPYManager::RegisterHandle(PyHandle* handle)
{
	if (!handle)
		return;

	for (int x=0; x<m_vHandles.Size(); x++)
	{
		if (m_vHandles[x]==handle)
			return;
	}

	m_vHandles.AddToTail(handle);

	//if we are late to the game
	if (m_bHandleInit)
		handle->InitHandle();
}

bp::object CGEPYManager::Exec(const char* buff)
{
	return bp::exec(buff, GetGloablDict(), GetGloablDict());
}

bp::object CGEPYManager::ExecFile(const char* name)
{
	char file[255];

	Q_snprintf(file, 255, "%s\\%s", GEPython()->GetRootPath(), name);

	char fullPath[255];
	filesystem->RelativePathToFullPath( file, "MOD", fullPath, 255);

	return bp::exec_file(fullPath, GetGloablDict(), GetGloablDict());
}


////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////




CON_COMMAND(py_exec, "Exec a python script (Path is relative to python script folder)")
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
	{
		Msg( "You must be a server admin to use that command\n" );
		return;
	}

	try
	{
		Msg(">>>Executing file at: scripts/python/%s\n", args[1]);
		GEPython()->ExecFile(args[1]);
		Msg("\n");
	}
	catch (...)
	{
		Msg("FAILED: Possibly an invalid directory?\n");
		HandlePythonException();
	}
}

CON_COMMAND(py, "Exec python")
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
	{
		Msg( "You must be a server admin to use that command\n" );
		return;
	}

	if ( args.ArgC() < 2 )
	{
		Msg("Executes python commands in the global namespace\n");
		return;
	}

	try
	{
		Msg(">>> %s\n", args[1]);
		GEPython()->Exec(args[1]);
		Msg("\n");
	}
	catch (...)
	{
		HandlePythonException();
	}
}




