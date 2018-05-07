#pragma once
#include "netsocketproxy.h"
class CTcpSocketProxy :
	public CNetSocketProxy
{
public:
	CTcpSocketProxy(void);
	virtual ~CTcpSocketProxy(void);

	virtual int SendData(const unsigned char* data, long len);
};

