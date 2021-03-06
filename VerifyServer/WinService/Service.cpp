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


// CWinService

// Implementation of service-oriented functionality of WinVNC


// Header

#include "Service.h"
#include "omnithread.h"


#include <lmcons.h>
#include <stdio.h>
#include <typeinfo>

#include "ServiceAppMain.h"
#include "DebugLogCollecter.h"

const TCHAR appName[]			= _T("VerifyServer");
const TCHAR runService[]		= _T("--service");
const TCHAR runServiceHelper[]	= _T("--servicehelper");

// OS-SPECIFIC ROUTINES

// Create an instance of the CWinService class to cause the static fields to be
// initialised properly

//CWinService init;

DWORD	g_platform_id;
BOOL	g_impersonating_user = FALSE;
HANDLE	g_impersonation_token = 0;
DWORD	g_version_major;
DWORD	g_version_minor;

ServiceAppMain			*g_pmain = 0;

#ifdef HORIZONLIVE
BOOL	g_nosettings_flag;
#endif

omni_thread * CWinService::workthread = 0; 
MyString CWinService::m_strServiceName = _T("");

CWinService::CWinService(ServiceAppMain *pmain) 
{
    OSVERSIONINFO osversioninfo;
    osversioninfo.dwOSVersionInfoSize = sizeof(osversioninfo);

    // Get the current OS version
    if (!GetVersionEx(&osversioninfo))
	    g_platform_id = 0;
    g_platform_id = osversioninfo.dwPlatformId;
	g_version_major = osversioninfo.dwMajorVersion;
	g_version_minor = osversioninfo.dwMinorVersion;

	g_pmain = pmain;

#ifdef HORIZONLIVE
	g_nosettings_flag = false;
#endif

}

CWinService::~CWinService()
{
	if (g_impersonating_user) {
		g_impersonating_user = FALSE;
		CloseHandle(g_impersonation_token);
		g_impersonation_token = 0;
	}
	
}

#ifdef HORIZONLIVE
void
CWinService::SetNoSettings(bool flag)
{
	g_nosettings_flag = flag;
}

BOOL CWinService::GetNoSettings()
{
	return g_nosettings_flag;
}

#endif



// IsWin95 - returns a BOOL indicating whether the current OS is Win95
BOOL
CWinService::IsWin95()
{
	return (g_platform_id == VER_PLATFORM_WIN32_WINDOWS);
}

// IsWinNT - returns a bool indicating whether the current OS is WinNT
BOOL
CWinService::IsWinNT()
{
	
	return (g_platform_id == VER_PLATFORM_WIN32_NT);
}

// Version info
DWORD
CWinService::VersionMajor()
{
	return g_version_major;
}

DWORD
CWinService::VersionMinor()
{
	return g_version_minor;
}


// Internal service state
SERVICE_STATUS          g_srvstatus;       // current status of the service
SERVICE_STATUS_HANDLE   g_hstatus;
DWORD                   g_error = 0;
DWORD					g_servicethread = NULL;
TCHAR*                  g_errortext[256];



// Forward defines of internal service functions
//void WINAPI ServiceMain(DWORD argc, TCHAR **argv);


void ServiceStop();
void WINAPI ServiceCtrl(DWORD ctrlcode);


BOOL ReportStatus(DWORD state, DWORD exitcode, DWORD waithint);

// ROUTINE TO QUERY WHETHER THIS PROCESS IS RUNNING AS A SERVICE OR NOT

BOOL	g_servicemode = FALSE;

BOOL
CWinService::RunningAsService()
{
	return g_servicemode;
}

// ROUTINE TO PROCESS AN INCOMING INSTANCE OF THE ABOVE MESSAGE


// SERVICE MAIN ROUTINE
int
CWinService::WinRunAsService(TCHAR *szAppName, TCHAR* szServiceName, TCHAR *szServiceDisplayName)
{
	typedef DWORD (WINAPI * RegisterServiceProc)(DWORD, DWORD);
	const ULONG RSP_SIMPLE_SERVICE = 0x00000001;
	const ULONG RSP_UNREGISTER_SERVICE = 0x00000000;

	g_servicemode = TRUE;
	MyString tmp(szServiceName);
	CWinService::m_strServiceName = tmp;
	service_log(FILE_LOG, _T("WinRunAsService"));
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
			//CWinService *pobj = CWinService::GetInstance();
			//if(pobj->winAppMain != NULL)
			{
				DWORD argc = 0;
				TCHAR** argv = NULL;			

				WinAppMain(argc, argv);
				
				//WinAppMain(argc, argv);
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
#ifdef _DEBUG
			ServiceMain(0, 0);
#else
			// Create a service entry table
			SERVICE_TABLE_ENTRY dispatchTable[] =
			{
				{(TCHAR*)szServiceName , (LPSERVICE_MAIN_FUNCTION)ServiceMain},
				{NULL, NULL}
			};

			// Call the service control dispatcher with our entry table
			if (!StartServiceCtrlDispatcher(dispatchTable))
			{
				DWORD err = ::GetLastError();
				service_log(FILE_LOG, _T("StartServiceCtrlDispatcher failed."));
				LogErrorMsg(_T("StartServiceCtrlDispatcher failed."));
			}
#endif // _DEBUG
			
		}
		break;

	default:
#ifdef _DEBUG
		ServiceMain(0, 0);
#else
		// Create a service entry table
		SERVICE_TABLE_ENTRY dispatchTable[] =
		{
			{(TCHAR*)szServiceName , (LPSERVICE_MAIN_FUNCTION)ServiceMain},
			{NULL, NULL}
		};

		// Call the service control dispatcher with our entry table
		if (!StartServiceCtrlDispatcher(dispatchTable))
		{
			DWORD err = ::GetLastError();
			service_log(FILE_LOG, _T("StartServiceCtrlDispatcher failed."));
			LogErrorMsg(_T("StartServiceCtrlDispatcher failed."));
		}
#endif // _DEBUG
		break;
	};

	return 0;
}

void* CWinService::WaitServiceMain(void ** returnval)
{
	if(CWinService::workthread != 0)
	{
		CWinService::workthread->join(returnval);
	}

	return &returnval;
}

// SERVICE MAIN ROUTINE
void WINAPI ServiceMain(DWORD argc, TCHAR**argv)
{
	// Register the service control handler
#ifndef _DEBUG

	g_hstatus = RegisterServiceCtrlHandler(CWinService::m_strServiceName.c_str() , ServiceCtrl);

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
		service_log(FILE_LOG, _T("ReportStatus failed SERVICE_START_PENDING"));
		ReportStatus(
			SERVICE_STOPPED,
			g_error,
			0);
		return;
	}
#endif //_DEBUG

	// Now start the service for real
	CWinService::workthread = omni_thread::create(ServiceWorkThread);
	//ServiceWorkThread(parg);
    return;
}

// SERVICE START ROUTINE - thread that calls WinAppMain
void* ServiceWorkThread(void *arg)
{
	
	// Save the current thread identifier
	g_servicethread = GetCurrentThreadId();

#ifndef _DEBUG
	// report the status to the service control manager.
	//
	if (!ReportStatus(
		SERVICE_RUNNING,       // service state
		NO_ERROR,              // exit code
		0))                    // wait hint
		return 0;
#endif //_DEBUG
	// RUN!

	WinAppMain(0, 0);


	// Mark that we're no longer running
	g_servicethread = NULL;

	// Tell the service manager that we've stopped.
    ReportStatus(
		SERVICE_STOPPED,
		g_error,
		0);

	return 0;
}

// SERVICE STOP ROUTINE - post a quit message to the relevant thread
void ServiceStop()
{
	// Post a quit message to the main service thread
	if (g_servicethread != NULL)
	{
		PostThreadMessage(g_servicethread, WM_QUIT, 0, 0);
	}

	if (g_pmain!= NULL)
	{
		g_pmain->MyPostMessage(WM_MYQUIT, 0, 0);
	}
}

// SERVICE INSTALL ROUTINE
int
CWinService::ReinstallService(TCHAR *szAppName, TCHAR* szServiceName, TCHAR *szServiceDisplayName, BOOL silent) {
	RemoveService(szAppName, szServiceName, 1);
	InstallService(szAppName, szServiceName,szServiceDisplayName, silent);
	return 0;
}

int
CWinService::InstallService(TCHAR *szAppName, TCHAR* szServiceName, TCHAR *szServiceDisplayName, BOOL silent)
{
	const int pathlength = 2048;
	TCHAR path[pathlength];
	TCHAR servicecmd[pathlength];

	MyString tmp(szServiceName);
	CWinService::m_strServiceName = tmp;

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
#ifdef _UNICODE

			char cmd[pathlength];
			::wcstombs(cmd, servicecmd, pathlength-1);
			char chAppName[256];
			::wcstombs(chAppName, szAppName, 256-1);
			// Add the VNCserviceHelper entry
			if (RegSetValueExA(runservices, chAppName, 0, REG_SZ,
				(unsigned char *)cmd, strlen(cmd)+1) != ERROR_SUCCESS)
			{
				RegCloseKey(runservices);

				break;
			}
#else
			// Attempt to add a WinService key
			if (RegSetValueEx(runservices, szAppName, 0, REG_SZ, (unsigned char *)servicecmd, lstrlen(servicecmd)+1) != ERROR_SUCCESS)
			{
				RegCloseKey(runservices);

				break;
			}
#endif
		

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
			service_log(FILE_LOG, _T("install service begin"));
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
				_T(""),			// dependencies
				NULL,						// LocalSystem account
				NULL);						// no password
			if (hservice == NULL)
			{
				DWORD error = GetLastError();
				
 				CloseServiceHandle(hsrvmanager);
				service_log(FILE_LOG, _T("CreateService service fail"));
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
//				TCHAR servicehelpercmd[pathlength];
//
//				// Append the service-helper-start flag to the end of the path:
//				if (lstrlen(path) + 4 + lstrlen(runServiceHelper) < pathlength)
//					MySprintf(servicehelpercmd, _T("\"%s\" %s"), path, runServiceHelper);
//				else
//					return 0;
//#ifdef _UNICODE
//
//				char cmd[pathlength];
//				::wcstombs(cmd, servicehelpercmd, pathlength-1);
//				char chAppName[256];
//				::wcstombs(chAppName, szAppName, 256-1);
//				// Add the VNCserviceHelper entry
//				if (RegSetValueExA(runapps, chAppName, 0, REG_SZ,
//					(unsigned char *)cmd, strlen(cmd)+1) != ERROR_SUCCESS)
//				{
//					;
//				}
//#else
//				if (RegSetValueEx(runapps, szAppName, 0, REG_SZ,
//					(unsigned char *)servicehelpercmd, lstrlen(servicehelpercmd)+1) != ERROR_SUCCESS)
//				{
//					;
//				}
//#endif
//				RegCloseKey(runapps);
				;
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
CWinService::RemoveService(TCHAR *szAppName, TCHAR *szServiceName, BOOL silent)
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
			//HKEY runapps;
			//if (RegOpenKey(HKEY_LOCAL_MACHINE, 
			//	_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),
			//	&runapps) == ERROR_SUCCESS)
			//{
			//	// Attempt to delete the WinVNC key
			//	if (RegDeleteValue(runapps, szAppName) != ERROR_SUCCESS)
			//	{
			//		;
			//	}
			//	RegCloseKey(runapps);
			//}

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
			{
				;
			}
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
	{
		service_log(FILE_LOG, _T("SetServiceStatus failed"));
		LogErrorMsg(_T("SetServiceStatus failed"));
	}
	else
	{
		const TCHAR strState[7][250] = { _T("SERVICE_STOPPED"),
			_T("SERVICE_START_PENDING"),
			_T("SERVICE_STOP_PENDING"),
			_T("SERVICE_RUNNING"),
			_T("SERVICE_CONTINUE_PENDING"),
			_T("SERVICE_PAUSE_PENDING"),
			_T("SERVICE_PAUSED")};
		MyString str_log(_T("SetServiceStatus"));
		str_log = str_log + _T(" ") + strState[state];
		service_log(FILE_LOG, str_log);
	}
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
    heventsrc = RegisterEventSource(NULL, CWinService::m_strServiceName.c_str());

	MySprintf(msgbuff, _T("%.200s error: %d"), CWinService::m_strServiceName.c_str(), g_error);
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
