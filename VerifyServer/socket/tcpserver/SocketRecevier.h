#ifndef __ESOP_SOCKET_RECEVICER_H__
#define __ESOP_SOCKET_RECEVICER_H__

#include "tcpsocket_define.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

class _TCPSOCKET_IMPORT_EXPORT_DLL_ CSocketRecevier
{
public:
	CSocketRecevier(void);
	virtual ~CSocketRecevier(void);

public:
	virtual void Recevie(DWORD uSokcetID, BYTE *rev_buf, UINT rev_len) = 0;
	virtual void connect_coming(DWORD socketid, unsigned int nport) = 0;
	virtual void unconnect_coming(DWORD socketid, unsigned int nport) = 0;
};

#endif //__ESOP_SOCKET_RECEVICER_H__