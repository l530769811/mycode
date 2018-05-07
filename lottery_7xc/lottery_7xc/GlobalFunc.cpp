#include "StdAfx.h"
#include "GlobalFunc.h"
#include "Shlwapi.h"
#include <stdio.h>

BOOL GetOSDisplayString( LPTSTR pszOS)
{
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;
	PGPI pGPI;
	BOOL bOsVersionInfoEx;
	DWORD dwType;

	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);

	if( ! bOsVersionInfoEx ) return 1;

	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

	pGNSI = (PGNSI) GetProcAddress(
		GetModuleHandle(TEXT("kernel32.dll")), 
		"GetNativeSystemInfo");
	if(NULL != pGNSI)
		pGNSI(&si);
	else GetSystemInfo(&si);

	if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && 
		osvi.dwMajorVersion > 4 )
	{
		StringCchCopy(pszOS, BUFSIZE, TEXT("Microsoft "));

		// Test for the specific product.

		if ( osvi.dwMajorVersion == 6 )
		{
			if( osvi.dwMinorVersion == 0 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows Vista "));
				else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 " ));
			}

			if ( osvi.dwMinorVersion == 1 || osvi.dwMinorVersion == 2 )
			{
				if ( osvi.wProductType == VER_NT_WORKSTATION && osvi.dwMinorVersion == 1)
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows 7 "));
				else
					if ( osvi.wProductType == VER_NT_WORKSTATION && osvi.dwMinorVersion == 2)
						StringCchCat(pszOS, BUFSIZE, TEXT("Windows 8 "));
					else 
						StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 R2 " ));
			}

			pGPI = (PGPI) GetProcAddress(
				GetModuleHandle(TEXT("kernel32.dll")), 
				"GetProductInfo");

			pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

			switch( dwType )
			{
			case PRODUCT_ULTIMATE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Ultimate Edition" ));
				break;
			case PRODUCT_PROFESSIONAL:
				StringCchCat(pszOS, BUFSIZE, TEXT("Professional" ));
				break;
			case PRODUCT_HOME_PREMIUM:
				StringCchCat(pszOS, BUFSIZE, TEXT("Home Premium Edition" ));
				break;
			case PRODUCT_HOME_BASIC:
				StringCchCat(pszOS, BUFSIZE, TEXT("Home Basic Edition" ));
				break;
			case PRODUCT_ENTERPRISE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition" ));
				break;
			case PRODUCT_BUSINESS:
				StringCchCat(pszOS, BUFSIZE, TEXT("Business Edition" ));
				break;
			case PRODUCT_STARTER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Starter Edition" ));
				break;
			case PRODUCT_CLUSTER_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Cluster Server Edition" ));
				break;
			case PRODUCT_DATACENTER_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition" ));
				break;
			case PRODUCT_DATACENTER_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition (core installation)" ));
				break;
			case PRODUCT_ENTERPRISE_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition" ));
				break;
			case PRODUCT_ENTERPRISE_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition (core installation)" ));
				break;
			case PRODUCT_ENTERPRISE_SERVER_IA64:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition for Itanium-based Systems" ));
				break;
			case PRODUCT_SMALLBUSINESS_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server" ));
				break;
			case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
				StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server Premium Edition" ));
				break;
			case PRODUCT_STANDARD_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition" ));
				break;
			case PRODUCT_STANDARD_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition (core installation)" ));
				break;
			case PRODUCT_WEB_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Web Server Edition" ));
				break;
			}
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
		{
			if( GetSystemMetrics(SM_SERVERR2) )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Server 2003 R2, "));
			else if ( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Storage Server 2003"));
			else if ( osvi.wSuiteMask & VER_SUITE_WH_SERVER )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Home Server"));
			else if( osvi.wProductType == VER_NT_WORKSTATION &&
				si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
			{
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows XP Professional x64 Edition"));
			}
			else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2003, "));

			// Test for the server type.
			if ( osvi.wProductType != VER_NT_WORKSTATION )
			{
				if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Edition for Itanium-based Systems" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise Edition for Itanium-based Systems" ));
				}

				else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter x64 Edition" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise x64 Edition" ));
					else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard x64 Edition" ));
				}

				else
				{
					if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Compute Cluster Edition" ));
					else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Edition" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise Edition" ));
					else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Web Edition" ));
					else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard Edition" ));
				}
			}
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		{
			StringCchCat(pszOS, BUFSIZE, TEXT("Windows XP "));
			if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Home Edition" ));
			else StringCchCat(pszOS, BUFSIZE, TEXT( "Professional" ));
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
		{
			StringCchCat(pszOS, BUFSIZE, TEXT("Windows 2000 "));

			if ( osvi.wProductType == VER_NT_WORKSTATION )
			{
				StringCchCat(pszOS, BUFSIZE, TEXT( "Professional" ));
			}
			else 
			{
				if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
					StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Server" ));
				else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					StringCchCat(pszOS, BUFSIZE, TEXT( "Advanced Server" ));
				else StringCchCat(pszOS, BUFSIZE, TEXT( "Server" ));
			}
		}

		// Include service pack (if any) and build number.

		if( _tcslen(osvi.szCSDVersion) > 0 )
		{
			StringCchCat(pszOS, BUFSIZE, TEXT(" ") );
			StringCchCat(pszOS, BUFSIZE, osvi.szCSDVersion);
		}

		TCHAR buf[80];

		StringCchPrintf( buf, 80, TEXT(" (build %d)"), osvi.dwBuildNumber);
		StringCchCat(pszOS, BUFSIZE, buf);

		if ( osvi.dwMajorVersion >= 6 )
		{
			if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				StringCchCat(pszOS, BUFSIZE, TEXT( ", 64-bit" ));
			else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
				StringCchCat(pszOS, BUFSIZE, TEXT(", 32-bit"));
		}

		return TRUE; 
	}

	else
	{  
		printf( "This sample does not support this version of Windows.\n");
		return FALSE;
	}
}

BOOL GetOSType(LPTSTR pszOS)
{
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;;
	BOOL bOsVersionInfoEx;

	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);

	if( ! bOsVersionInfoEx ) return TRUE;

	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

	pGNSI = (PGNSI) GetProcAddress(
		GetModuleHandle(TEXT("kernel32.dll")), 
		"GetNativeSystemInfo");
	if(NULL != pGNSI)
		pGNSI(&si);
	else GetSystemInfo(&si);

	if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && 
		osvi.dwMajorVersion > 4 )
	{
		// Test for the specific product.

		if ( osvi.dwMajorVersion == 6 )
		{
			if( osvi.dwMinorVersion == 0 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows Vista"));
				else StringCchCat(pszOS, BUFSIZE, TEXT("Server 2008" ));
			}

			if ( osvi.dwMinorVersion == 1 || osvi.dwMinorVersion == 2 )
			{
				if ( osvi.wProductType == VER_NT_WORKSTATION && osvi.dwMinorVersion == 1)
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows 7"));
				else
					if ( osvi.wProductType == VER_NT_WORKSTATION && osvi.dwMinorVersion == 2)
						StringCchCat(pszOS, BUFSIZE, TEXT("Windows 8"));
					else 
						StringCchCat(pszOS, BUFSIZE, TEXT("Server 2012" ));
			}			
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
		{
			if( GetSystemMetrics(SM_SERVERR2) )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Server 2003 R2,"));
			else if ( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Server 2003"));
			else if ( osvi.wSuiteMask & VER_SUITE_WH_SERVER )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Server"));
			else if( osvi.wProductType == VER_NT_WORKSTATION &&
				si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
			{
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows XP"));
			}
			else StringCchCat(pszOS, BUFSIZE, TEXT("Server 2003,"));
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		{
			StringCchCat(pszOS, BUFSIZE, TEXT("Windows XP"));
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
		{
			StringCchCat(pszOS, BUFSIZE, TEXT("Windows 2000 "));
		}

		if ( osvi.dwMajorVersion >= 6 )
		{
			if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				StringCchCat(pszOS, BUFSIZE, TEXT( "-64bit" ));
			else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
				StringCchCat(pszOS, BUFSIZE, TEXT("-32bit"));
		}

		return TRUE; 
	}

	else
	{  
		printf( "This sample does not support this version of Windows.\n");
		return FALSE;
	}
}
WIN_OS_BIT_TYPE GetOSBitType()
{
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;
	BOOL bOsVersionInfoEx;
	WIN_OS_BIT_TYPE os_bit_type = UNKOWN;

	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);

	if( ! bOsVersionInfoEx ) return os_bit_type;

	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

	pGNSI = (PGNSI) GetProcAddress(
		GetModuleHandle(TEXT("kernel32.dll")), 
		"GetNativeSystemInfo");
	if(NULL != pGNSI)
		pGNSI(&si);
	else GetSystemInfo(&si);

	if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && 
		osvi.dwMajorVersion > 4 )
	{
		if ( osvi.dwMajorVersion >= 6 )
		{
			if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				os_bit_type = X64;
			else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
				os_bit_type = X86;
		} 
	}

	else
	{  
		printf( "This sample does not support this version of Windows.\n");
	
	}

	return os_bit_type;
}


MyString GetModuleFilePath()
{
	TCHAR str[MAX_PATH] = {0};
	MyString csBasePath;
	::GetModuleFileName(NULL, str, MAX_PATH);
	csBasePath = str;

	int nPos = -1;
	nPos = csBasePath.rfind(_T('\\'));
	csBasePath = csBasePath.substr(0, nPos);
	return csBasePath;
}

#ifdef _AFXDLL



TCHAR* GetLocalIP(TCHAR* pIp)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	char Name[255] = {0};//定义用于存放获得的主机名的变量 
	char *IP = 0;//定义IP地址变量 
	char chIP[MAX_PATH]  = {0};
	PHOSTENT hostinfo;
	if(gethostname (Name, sizeof(Name)) == 0)
	{ 
		//如果成功地将本地主机名存放入由name参数指定的缓冲区中 
		if((hostinfo = gethostbyname(Name)) != NULL) 
		{ 
			//这是获取主机名，如果获得主机名成功的话，将返回一个指针，指向hostinfo，hostinfo 
			//为PHOSTENT型的变量，下面即将用到这个结构体 
			IP = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
			int nIpLen = ::strlen(IP);

			::memcpy(chIP, IP, nIpLen);
			DWORD dwMun = ::MultiByteToWideChar(CP_ACP, 
				0, 
				chIP,
				-1, 
				NULL,
				0);
			dwMun = dwMun * sizeof(TCHAR);
			::MultiByteToWideChar(CP_ACP, 0,
				chIP, -1, pIp, dwMun);
		}
		else
		{
			//WRITE_LOG("gethostname Fail");
		}
	}
	else
	{
		long err = ::GetLastError();
		int i = 0;
	}
	
	WSACleanup();
	return pIp;
}
#else

#endif //_AFXDLL


int UnicodeToUtf8(const TCHAR *unicode, char *uft8, int len)
{
	int utf8size = 0;
#ifdef _UNICODE
	utf8size = ::WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	if (utf8size > len)
	{
		return NULL;
	}


	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)(LPCWSTR)unicode, -1, uft8, sizeof(char *)*utf8size, NULL, NULL);
#else
	strcpy(uft8, unicode);
	utf8size = len;
#endif //_UNICODE
	return utf8size;
}

TCHAR* Utf8ToUnicode(const char *uft8, TCHAR *unicode, int len)
{
#ifdef _UNICODE
	
	int nLen = 0;
	nLen = MultiByteToWideChar(CP_UTF8, 0, (char*)uft8, -1, NULL, 0);
	if (len < nLen)
	{
		return NULL;
	}
	::memset(unicode, 0, len * sizeof(TCHAR));
	::MultiByteToWideChar(CP_UTF8, 0, (char*)uft8, -1, unicode, nLen);

#else
	strcpy(unicode, uft8);
#endif //_UNICODE
	return unicode;
}


