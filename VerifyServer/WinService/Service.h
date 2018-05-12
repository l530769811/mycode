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

// SERVICE-MODE CODE

// This class provides access to service-oriented routines, under both
// Windows NT and Windows 95.  Some routines only operate under one
// OS, others operate under any OS.

class CWinService;

#if (!defined(_WINVNC_VNCSERVICE))
#define _WINVNC_VNCSERVICE

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <tchar.h>

#include "SingleModeTemplete.h"

#ifndef UNICODE
#define MySprintf sprintf
#else
#define MySprintf swprintf
#endif

class ServiceAppMain;
struct CArg{
	DWORD argc;
	TCHAR**argv;
};


//extern int WinAppMain(DWORD argc, TCHAR** argv);

// The NT-specific code wrapper class
class omni_thread;
class CWinService
{
public:
	
	CWinService(ServiceAppMain *pmain);
	~CWinService();
	friend void WINAPI ServiceMain(DWORD argc, TCHAR **argv);
	// SERVICE INSTALL & START FUNCTIONS

	// Routine called by WinMain to cause WinVNC to be installed
	// as a service.
	static int WinRunAsService();

	// Routine to install the WinVNC service on the local machine
	static int InstallService(TCHAR *szAppName, TCHAR* szServiceName, TCHAR *szServiceDisplayName, BOOL silent=0);
	static int ReinstallService(TCHAR *szAppName,TCHAR* szServiceName, TCHAR *szServiceDisplayName, BOOL silent=0);

	// Routine to remove the WinVNC service from the local machine
	static int RemoveService(TCHAR *szAppName, TCHAR* szServiceName, BOOL silent=0);

	// SERVICE SUPPORT FUNCTIONS


	// Routines to establish which OS we're running on
	static BOOL IsWin95();
	static BOOL IsWinNT();
	static DWORD VersionMajor();
	static DWORD VersionMinor();

	// Routine to establish whether the current instance is running
	// as a service or not
	static BOOL RunningAsService();

	static void* WaitServiceMain(void** returnval);

#ifdef HORIZONLIVE
	static void SetNoSettings(bool flag);
	static BOOL GetNoSettings();
#endif
private:
	//typedef int (*WinAppMain)(DWORD argc, TCHAR** argv);
	//static WinAppMain *winAppMain;
	static omni_thread *workthread;

private:
	friend void* ServiceWorkThread(void *arg);
};

#endif
