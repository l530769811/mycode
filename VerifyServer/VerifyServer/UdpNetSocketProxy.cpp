#include "UdpNetSocketProxy.h"
#include "UDPSocket.h"


CUdpNetSocketProxy::CUdpNetSocketProxy(void)
	:m_bout_socket(false)
{
	m_psocket = (0);
}
CUdpNetSocketProxy::CUdpNetSocketProxy(CUDPSocket *psocket, MyString ip_to, long port_to)
	: m_bout_socket(true)
	, m_psocket(psocket)
	, m_strIpTo(ip_to)
	, m_portTo(port_to)
{

}


CUdpNetSocketProxy::~CUdpNetSocketProxy(void)
{
	if (m_bout_socket==false)
	{
		delete m_psocket;
	}
}


int CUdpNetSocketProxy::SendData(const unsigned char* data, long len)
{
	int rlen = 0;
	if(m_psocket->SendData((unsigned char *)data, len, m_strIpTo.c_str(), (unsigned short)m_portTo) == true)
		rlen = len;
	return rlen;
}