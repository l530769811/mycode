#include "StdAfx.h"
#include "SocketRecevier.h"
#include "TcpServer.h"


CSocketRecevier::CSocketRecevier(void)
	: m_psocket(0)
{
}


CSocketRecevier::~CSocketRecevier(void)
{
}

void CSocketRecevier::BindSocket(CTcpServer *psocket)
{
	m_psocket = psocket;
}

CTcpServer* CSocketRecevier::GetSocket() const 
{
	return m_psocket;
}

UINT CSocketRecevier::Send(DWORD uSocketID, const BYTE *send_buf, UINT len)
{
	UINT ret_len = 0;
	if(m_psocket!=0)
	{		
		if(m_psocket->Send(uSocketID, send_buf, len, TRUE))
		{
			ret_len = len;
		}
	}
	return ret_len;
}
