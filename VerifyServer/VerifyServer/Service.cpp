//  Copyright (C) 2002 RealVNC Ltd. All Rights Reserved.
//  Copyright (C) 1999 AT&T Laboratories Cambridge. All Rights Reserved.
//  Copyright (C) 2009 GlavSoft LLC. All Rights Reserved.
//
//  This file is part of the VNC system.
//
//  The VNC system is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
//  USA.
//
// TightVNC distribution homepage on the Web: http://www.tightvnc.com/
//
// If the source code for the VNC system is not available from the place 
// whence you received this file, check http://www.uk.research.att.com/vnc or contact
// the authors on vnc@uk.research.att.com for information on obtaining it.


// vncService

// Implementation of service-oriented functionality of WinVNC


// Header

#include "Service.h"
#include "omnithread.h"


#include <lmcons.h>
#include <stdio.h>
#include <typeinfo>

#include "ServiceAppMain.h"

// Error message logging
void LogErrorMsg(TCHAR *message);

const TCHAR runService[]		= _T("-service");
const TCHAR runServiceHelper[]	= _T("-servicehelper");

// OS-SPECIFIC ROUTINES

// Create an instance of the vncService class to cause the static fields to be
// initialised properly

vncService init;

DWORD	g_platform_id;
BOOL	g_impersonating_user = FALSE;
HANDLE	g_impersonation_token = 0;
DWORD	g_version_major;
DWORD	g_version_minor;

#ifdef HORIZONLIVE
BOOL	g_nosettings_flag;
#endif

vncService::vncService()
{
    OSVERSIONINFO osversioninfo;
    osversioninfo.dwOSVersionInfoSize = sizeof(osversioninfo);

    // Get the current OS version
    if (!GetVersionEx(&osversioninfo))
	    g_platform_id = 0;
    g_platform_id = osversioninfo.dwPlatformId;
	g_version_major = osversioninfo.dwMajorVersion;
	g_version_minor = osversioninfo.dwMinorVersion;
#ifdef HORIZONLIVE
	g_nosettings_flag = false;
#endif

}

vncService::~vncService()
{
	if (g_impersonating_user) {
		g_impersonating_user = FALSE;
		CloseHandle(g_impersonation_token);
		g_impersonation_token = 0;
	}
}

#ifdef HORIZONLIVE
void
vncService::SetNoSettings(bool flag)
{
	g_nosettings_flag = flag;
}

BOOL vncService::GetNoSettings()
{
	return g_nosettings_flag;
}

#endif



// IsWin95 - returns a BOOL indicating whether the current OS is Win95
BOOL
vncService::IsWin95()
{
	return (g_platform_id == VER_PLATFORM_WIN32_WINDOWS);
}

// IsWinNT - returns a bool indicating whether the current OS is WinNT
BOOL
vncService::IsWinNT()
{
	return (g_platform_id == VER_PLATFORM_WIN32_NT);
}

// Version info
DWORD
vncService::VersionMajor()
{
	return g_version_major;
}

DWORD
vncService::VersionMinor()
{
	return g_version_minor;
}




// SERVICE-MODE ROUTINES

// Service-mode defines:

// Executable name
#define APPNAME            _T("winvnc")

// Internal service name
#define VNCSERVICENAME        _T("winvnc")

// Displayed service name
#define VNCSERVICEDISPLAYNAME _T("VNC Server")

// List of other required services ("dependency 1\0dependency 2\0\0")
// *** These need filling in properly
#define VNCDEPENDENCIES       _T("")

// Internal service state
SERVICE_STATUS          g_srvstatus;       // current status of the service
SERVICE_STATUS_HANDLE   g_hstatus;
DWORD                   g_error = 0;
DWORD					g_servicethread = NULL;
TCHAR*                   g_errortext[256];

// Forward defines of internal service functions
void WINAPI ServiceMain(DWORD argc, TCHAR **argv);


void ServiceStop();
void WINAPI ServiceCtrl(DWORD ctrlcode);

bool WINAPI CtrlHandler (DWORD ctrltype);

BOOL ReportStatus(DWORD state, DWORD exitcode, DWORD waithint);

// ROUTINE TO QUERY WHETHER THIS PROCESS IS RUNNING AS A SERVICE OR NOT

BOOL	g_servicemode = FALSE;

BOOL
vncService::RunningAsService()
{
	return g_servicemode;
}

// ROUTINE TO PROCESS AN INCOMING INSTANCE OF THE ABOVE MESSAGE


// SERVICE MAIN ROUTINE
int
vncService::WinRunAsService()
{
	typedef DWORD (WINAPI * RegisterServiceProc)(DWORD, DWORD);
	const ULONG RSP_SIMPLE_SERVICE = 0x00000001;
	const ULONG RSP_UNREGISTER_SERVICE = 0x00000000;

	g_servicemode = TRUE;

	// How to run as a service depends upon the OS being used
	switch (g_platform_id)
	{

		// Windows 95/98
	case VER_PLATFORM_WIN32_WINDOWS:
		{
			// Obtain a handle to the kernel library
			HINSTANCE kerneldll = LoadLibrary(_T("KERNEL32.DLL"));
			if (kerneldll == NULL)
				break;

			// And find the RegisterServiceProcess function
			RegisterServiceProc RegisterService;
			RegisterService = (RegisterServiceProc) GetProcAddress(kerneldll, "RegisterServiceProcess");
			if (RegisterService == NULL)
				break;

			// Register this process with the OS as a service!
			RegisterService(NULL, RSP_SIMPLE_SERVICE);

			// Run the service itself
			//vncService *pobj = vncService::GetInstance();
			//if(pobj->winAppMain != NULL)
			{
				DWORD argc = 0;
				TCHAR** argv = NULL;			

				//vncService::winAppMain(argc, argv);
				WinAppMain(argc, argv);
			}

			// Then remove the service from the system service table
			RegisterService(NULL, RSP_UNREGISTER_SERVICE);

			// Free the kernel library
			FreeLibrary(kerneldll);

			// *** If we don't kill the process directly here, then 
			// for some reason, WinVNC crashes...
			// *** Is this now fixed (with the stdcall patch above)?
			//ExitProcess(0);
		}
		break;

		// Windows NT
	case VER_PLATFORM_WIN32_NT:
		{
			// Create a service entry table
			SERVICE_TABLE_ENTRY dispatchTable[] =
		    {
				{VNCSERVICENAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
				{NULL, NULL}
			};

			// Call the service control dispatcher with our entry table
			if (!StartServiceCtrlDispatcher(dispatchTable))
				LogErrorMsg(_T("StartServiceCtrlDispatcher failed."));
		}
		break;

	};

	return 0;
}

// SERVICE MAIN ROUTINE
void WINAPI ServiceMain(DWORD argc, TCHAR**argv)
{
	// Register the service control handler
    g_hstatus = RegisterServiceCtrlHandler(VNCSERVICENAME, ServiceCtrl);

    if (g_hstatus == 0)
		return;

	// Set up some standard service state values
    g_srvstatus.dwServiceType = SERVICE_WIN32 | SERVICE_INTERACTIVE_PROCESS;
    g_srvstatus.dwServiceSpecificExitCode = 0;

	// Give this status to the SCM
    if (!ReportStatus(
        SERVICE_START_PENDING,	// Service state
        NO_ERROR,				// Exit code type
        15000))					// Hint as to how long WinVNC should have hung before you assume error
	{
        ReportStatus(
			SERVICE_STOPPED,
			g_error,
            0);
		return;
	}

	// Now start the service for real
	CArg *parg = new CArg();
	parg->argc = argc;
	parg->argv = new TCHAR*[argc];
	for (int i=0; i<argc; i++)
	{
		argv[i] = new TCHAR[256];
		memcpy(argv[1], argv[i], 256);
	}
	omni_thread *workthread = omni_thread::create(ServiceWorkThread, parg);
    return;
}

// SERVICE START ROUTINE - thread that calls WinAppMain
void ServiceWorkThread(void *arg)
{
	CArg *p = NULL;
	if(typeid(arg) == typeid(CArg*))
		p= (CArg*)arg;
	// Save the current thread identifier
	g_servicethread = GetCurrentThreadId();

    // report the status to the service control manager.
    //
    if (!ReportStatus(
        SERVICE_RUNNING,       // service state
        NO_ERROR,              // exit code
        0))                    // wait hint
		return;

	// RUN!
	//vncService *pobj = vncService::GetInstance();
	//if(pobj->winAppMain != NULL)
	{
		DWORD argc = 0;
		TCHAR** argv = NULL;
		if (p != NULL)
		{
			argc = p->argc;
			argv = p->argv;
		}
		WinAppMain(argc, argv);
		//vncService::winAppMain(argc, argv);
	}


	// Mark that we're no longer running
	g_servicethread = NULL;

	// Tell the service manager that we've stopped.
    ReportStatus(
		SERVICE_STOPPED,
		g_error,
		0);
}

// SERVICE STOP ROUTINE - post a quit message to the relevant thread
void ServiceStop()
{
	// Post a quit message to the main service thread
	if (g_servicethread != NULL)
	{
		PostThreadMessage(g_servicethread, WM_QUIT, 0, 0);
	}
}

// SERVICE INSTALL ROUTINE
int
vncService::ReinstallService(TCHAR *szAppName, TCHAR* szServiceName, TCHAR *szServiceDisplayName, BOOL silent) {
	RemoveService(szAppName, szServiceName, 1);
	InstallService(szAppName, szServiceName,szServiceDisplayName, silent);
	return 0;
}

int
vncService::InstallService(TCHAR *szAppName, TCHAR* szServiceName, TCHAR *szServiceDisplayName, BOOL silent)
{
	const int pathlength = 2048;
	TCHAR path[pathlength];
	TCHAR servicecmd[pathlength];

	// Get the filename of this executable
    if (GetModuleFileName(NULL, path, pathlength-(lstrlen(runService)+2)) == 0) {
		
		return 0;
    }

	// Append the service-start flag to the end of the path:
	if (lstrlen(path) + 4 + lstrlen(runService) < pathlength)
		MySprintf(servicecmd, _T("\"%s\" %s"), path, runService);
	else
		return 0;

	// How to add the WinVNC service depends upon the OS
	switch (g_platform_id)
	{

		// Windows 95/98
	case VER_PLATFORM_WIN32_WINDOWS:
		{
			// Locate the RunService registry entry
			HKEY runservices;
			if (RegCreateKey(HKEY_LOCAL_MACHINE, 
				_T("Software\\Microsoft\\Windows\\CurrentVersion\\RunServices"),
				&runservices) != ERROR_SUCCESS)
			{
				
				break;
			}

			// Attempt to add a WinVNC key
			if (RegSetValueEx(runservices, szAppName, 0, REG_SZ, (unsigned char *)servicecmd, lstrlen(servicecmd)+1) != ERROR_SUCCESS)
			{
				RegCloseKey(runservices);
				
				break;
			}

			RegCloseKey(runservices);

			// We have successfully installed the service!
			

			// Run the service...
			STARTUPINFO si;
			si.cb = sizeof(si);
			si.cbReserved2 = 0;
			si.lpReserved = NULL;
			si.lpReserved2 = NULL;
			si.dwFlags = 0;
			si.lpTitle = NULL;
			PROCESS_INFORMATION pi;
			if (!CreateProcess(
				NULL, servicecmd,							// Program name & path
				NULL, NULL,									// Security attributes
				FALSE,										// Inherit handles?
				NORMAL_PRIORITY_CLASS,						// Extra startup flags
				NULL,										// Environment table
				NULL,										// Current directory
				&si,
				&pi
				))
			{
				
				break;
			}
		}
		break;

		// Windows NT
	case VER_PLATFORM_WIN32_NT:
		{
			SC_HANDLE   hservice;
		    SC_HANDLE   hsrvmanager;

			// Open the default, local Service Control Manager database
		    hsrvmanager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (hsrvmanager == NULL)
			{
				
				break;
			}

			// Create an entry for the WinVNC service
			hservice = CreateService(
				hsrvmanager,				// SCManager database
				szServiceName,				// name of service
				szServiceDisplayName,		// name to display
				SERVICE_ALL_ACCESS,			// desired access
				SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
											// service type
				SERVICE_AUTO_START,			// start type
				SERVICE_ERROR_NORMAL,		// error control type
				servicecmd,					// service's binary
				NULL,						// no load ordering group
				NULL,						// no tag identifier
				VNCDEPENDENCIES,			// dependencies
				NULL,						// LocalSystem account
				NULL);						// no password
			if (hservice == NULL)
			{
				DWORD error = GetLastError();
				
 				CloseServiceHandle(hsrvmanager);
				break;
			}
			CloseServiceHandle(hsrvmanager);
			CloseServiceHandle(hservice);

			// Now install the servicehelper registry setting...
			// Locate the RunService registry entry
			HKEY runapps;
			if (RegCreateKey(HKEY_LOCAL_MACHINE, 
				_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),
				&runapps) != ERROR_SUCCESS)
			{
				;
			} else {
				TCHAR servicehelpercmd[pathlength];

				// Append the service-helper-start flag to the end of the path:
				if (lstrlen(path) + 4 + lstrlen(runServiceHelper) < pathlength)
					MySprintf(servicehelpercmd, _T("\"%s\" %s"), path, runServiceHelper);
				else
					return 0;

				// Add the VNCserviceHelper entry
				if (RegSetValueEx(runapps, szAppName, 0, REG_SZ,
					(unsigned char *)servicehelpercmd, lstrlen(servicehelpercmd)+1) != ERROR_SUCCESS)
				{
					;
				}
				RegCloseKey(runapps);
			}

			// Everything went fine
			if (!silent) {
				;
			}
		}
		break;
	};

	return 0;
}

// SERVICE REMOVE ROUTINE
int
vncService::RemoveService(TCHAR *szAppName, TCHAR *szServiceName, BOOL silent)
{
	// How to remove the WinVNC service depends upon the OS
	switch (g_platform_id)
	{

		// Windows 95/98
	case VER_PLATFORM_WIN32_WINDOWS:
		{
			// Locate the RunService registry entry
			HKEY runservices;
			if (RegOpenKey(HKEY_LOCAL_MACHINE, 
				_T("Software\\Microsoft\\Windows\\CurrentVersion\\RunServices"),
				&runservices) != ERROR_SUCCESS)
			{
				;
			}
			else
			{
				// Attempt to delete the WinVNC key
				if (RegDeleteValue(runservices, szAppName) != ERROR_SUCCESS)
				{
					RegCloseKey(runservices);
					
				}

				RegCloseKey(runservices);
				break;
			}

		
			
		}
		break;

		// Windows NT
	case VER_PLATFORM_WIN32_NT:
		{
			SC_HANDLE   hservice;
			SC_HANDLE   hsrvmanager;

			// Attempt to remove the service-helper hook
			HKEY runapps;
			if (RegOpenKey(HKEY_LOCAL_MACHINE, 
				_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),
				&runapps) == ERROR_SUCCESS)
			{
				// Attempt to delete the WinVNC key
				if (RegDeleteValue(runapps, szAppName) != ERROR_SUCCESS)
				{
					;
				}
				RegCloseKey(runapps);
			}

			// Open the SCM
		    hsrvmanager = OpenSCManager(
                        NULL,                   // machine (NULL == local)
                        NULL,                   // database (NULL == default)
                        SC_MANAGER_ALL_ACCESS   // access required
                        );
		    if (hsrvmanager)
		    {
		        hservice = OpenService(hsrvmanager, szServiceName, SERVICE_ALL_ACCESS);

				if (hservice != NULL)
				{
					SERVICE_STATUS status;

					// Try to stop the WinVNC service
					if (ControlService(hservice, SERVICE_CONTROL_STOP, &status))
					{
						while(QueryServiceStatus(hservice, &status))
						{
							if (status.dwCurrentState == SERVICE_STOP_PENDING)
								Sleep(1000);
							else
								break;
						}

						if (status.dwCurrentState != SERVICE_STOPPED) {
							;
						}
					}

					// Now remove the service from the SCM
					if (DeleteService(hservice)) {
						;
					} else {
						DWORD error = GetLastError();
						if (error == ERROR_SERVICE_MARKED_FOR_DELETE) {
							;
						} else {
							;
						}
					}
					CloseServiceHandle(hservice);
				}
				else if (!silent)
					

				CloseServiceHandle(hsrvmanager);
			}
			else if (!silent)
				;
		}
		break;
	};
	return 0;
}

// USEFUL SERVICE SUPPORT ROUTINES

// Service control routine
void WINAPI ServiceCtrl(DWORD ctrlcode)
{
	// What control code have we been sent?
    switch(ctrlcode)
    {

	case SERVICE_CONTROL_STOP:
		// STOP : The service must stop
		g_srvstatus.dwCurrentState = SERVICE_STOP_PENDING;
        ServiceStop();
        break;

    case SERVICE_CONTROL_INTERROGATE:
		// QUERY : Service control manager just wants to know our state
		break;

	default:
		// Control code not recognised
		break;

    }

	// Tell the control manager what we're up to.
    ReportStatus(g_srvstatus.dwCurrentState, NO_ERROR, 0);
}

// Service manager status reporting
BOOL ReportStatus(DWORD state,
				  DWORD exitcode,
				  DWORD waithint)
{
	static DWORD checkpoint = 1;
	BOOL result = TRUE;

	// If we're in the start state then we don't want the control manager
	// sending us control messages because they'll confuse us.
    if (state == SERVICE_START_PENDING)
		g_srvstatus.dwControlsAccepted = 0;
	else
		g_srvstatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	// Save the new status we've been given
	g_srvstatus.dwCurrentState = state;
	g_srvstatus.dwWin32ExitCode = exitcode;
	g_srvstatus.dwWaitHint = waithint;

	// Update the checkpoint variable to let the SCM know that we
	// haven't died if requests take a long time
	if ((state == SERVICE_RUNNING) || (state == SERVICE_STOPPED))
		g_srvstatus.dwCheckPoint = 0;
	else
        g_srvstatus.dwCheckPoint = checkpoint++;

	// Tell the SCM our new status
	if (!(result = SetServiceStatus(g_hstatus, &g_srvstatus)))
		LogErrorMsg(_T("SetServiceStatus failed"));

    return result;
}

// Error reporting
void LogErrorMsg(TCHAR *message)
{
    TCHAR	msgbuff[256];
    HANDLE	heventsrc;
    TCHAR *	strings[2];

	// Save the error code
	g_error = GetLastError();

	// Use event logging to log the error
    heventsrc = RegisterEventSource(NULL, VNCSERVICENAME);

	MySprintf(msgbuff, _T("%.200s error: %d"), VNCSERVICENAME, g_error);
    strings[0] = msgbuff;
    strings[1] = message;

	if (heventsrc != NULL)
	{
		

		ReportEvent(
			heventsrc,				// handle of event source
			EVENTLOG_ERROR_TYPE,	// event type
			0,						// event category
			0,						// event ID
			NULL,					// current user's SID
			2,						// strings in 'strings'
			0,						// no bytes of raw data
			(const TCHAR **)strings,	// array of error strings
			NULL);					// no raw data

		DeregisterEventSource(heventsrc);
	}
}
