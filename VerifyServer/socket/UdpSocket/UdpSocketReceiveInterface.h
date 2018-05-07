#ifndef __SOCKET_RECEIVE_INTERFACE_H__
#define __SOCKET_RECEIVE_INTERFACE_H__
#include "UdpSocket_define.h"

class _UDPSOCKET_IMPORT_EXPORT_DLL_ CUdpSocketReceiveInterface
{
public:
	CUdpSocketReceiveInterface(void);
	virtual ~CUdpSocketReceiveInterface(void);

	virtual int rev_data(const unsigned char * data, long len, char *ip_form, unsigned short port_from) = 0;
};

#endif //__SOCKET_RECEIVE_INTERFACE_H__