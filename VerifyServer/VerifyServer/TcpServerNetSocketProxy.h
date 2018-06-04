#ifndef _TCPSERVERNETSOCKETPROXY_H__
#define _TCPSERVERNETSOCKETPROXY_H__

#include "netsocketproxy.h"

class CSocketRecevier;
class CTcpServerNetSocketProxy :
	public CNetSocketProxy
{
public:
	CTcpServerNetSocketProxy(CSocketRecevier *psocket, unsigned long socket_id);
	virtual ~CTcpServerNetSocketProxy(void);

public:
	virtual int SendData(const unsigned char* data, long len);

private:
	CSocketRecevier *m_socket;
	unsigned long m_socket_id;
};

#endif _TCPSERVERNETSOCKETPROXY_H__