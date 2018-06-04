#include "TcpServerNetSocketProxy.h"
#include "SocketRecevier.h"


CTcpServerNetSocketProxy::CTcpServerNetSocketProxy(CSocketRecevier *psocket, unsigned long socket_id)
	: m_socket(psocket)
	, m_socket_id(socket_id)
{
}


CTcpServerNetSocketProxy::~CTcpServerNetSocketProxy(void)
{
}

int CTcpServerNetSocketProxy::SendData(const unsigned char* data, long len)
{
	int nlen = 0;
	if(m_socket!=0)
	{
		m_socket->Send(m_socket_id, data, len);
	}

	return nlen;
}
