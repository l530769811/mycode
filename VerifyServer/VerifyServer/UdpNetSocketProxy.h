#pragma once
#include "netsocketproxy.h"
#include <my_assist_define.h>

class CUDPSocket;
class CUdpNetSocketProxy :
	public CNetSocketProxy
{
public:
	CUdpNetSocketProxy(void);
	CUdpNetSocketProxy(CUDPSocket *psocket, MyString ip_to, long port_to);
	virtual ~CUdpNetSocketProxy(void);
	virtual int SendData(const unsigned char* data, long len);

private:
	const bool m_bout_socket;
	CUDPSocket *m_psocket;
	MyString m_strIpTo;
	long m_portTo;
};

