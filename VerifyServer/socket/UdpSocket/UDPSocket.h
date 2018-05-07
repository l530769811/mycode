#ifndef __CC500_UDP_SERVER_H__ 
#define __CC500_UDP_SERVER_H__

#ifdef WIN32

#ifndef __CC500_INCLUDE_WINDOWS_HEADER_H__
#define __CC500_INCLUDE_WINDOWS_HEADER_H__
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif //__CC500_INCLUDE_WINDOWS_HEADER_H__

#include <WS2tcpip.h>
#include <MSWSock.h>
#include <MSTcpIP.h>
#include <iostream>   
#include <stdio.h>       
#include <string.h> 
#include <process.h>
#include "UdpSocket_define.h"
using namespace std;


#else

#include <iostream>   
#include <stdio.h>   
#include <sys/socket.h>   
#include <unistd.h>   
#include <sys/types.h>   
#include <netdb.h>   
#include <netinet/in.h>   
#include <arpa/inet.h>   
#include <string.h> 
#include <pthread.h>

using namespace std; 

#define SOCKET int
#define  CRITICAL_SECTION   pthread_mutex_t
#endif //endif WIN32


#ifdef WIN32 
//windows program

#define RESULT_THREAD unsigned int __stdcall

#else
//linux program

#define RESULT_THREAD void*

#define InitializeCriticalSection(lock) pthread_mutex_init(lock, NULL)
#define DeleteCriticalSection(lock) pthread_mutex_destroy(lock)
#endif

typedef void (*RECEVIE_FUNC)(BYTE *pbyte, int nlen);
class CUdpSocketReceiveInterface;
class _UDPSOCKET_IMPORT_EXPORT_DLL_ CUDPSocket
{
public:
	CUDPSocket(CUdpSocketReceiveInterface *rev_interface);	
	~CUDPSocket(void);
	bool Start(unsigned short usPort);
	bool Stop();

	

	//the send tp port;
	bool SendData(BYTE chData[], int nLen, const TCHAR * desIp, const unsigned short desPort);
	//the send appoint SO_BROADCAST 
	bool SendData(BYTE chData[], int nLen);
	//the send appoint the aim;
	bool SendData(BYTE chData[], int nLen, const struct sockaddr_in &sendto);
private:
	bool _InitBroadcastSocket(unsigned short usPort);
	bool _InitMulticastSocket(unsigned short usPort);

	void _WantStopThread();
	bool _IsWantStopThread();
	void _TranData(char chData[], int len);
	//void _TranHeadData(char chData[], int len);
	//void _TranContentData(char chData[], int len);
	bool _StartRecive(unsigned short usDesPort);

	static RESULT_THREAD _RevThread(void* pParam);

	static int GlobalUnicodeToUtf8(const TCHAR *unicode, char *uft8, int len);

private:
	SOCKET m_sockServer;
	struct sockaddr_in m_addrSend;
	struct sockaddr_in m_addrRevice;
	bool m_bStopUDPRevThread;
	bool m_bFinishUDPSend;

	CUdpSocketReceiveInterface *m_pRevInterface;
	
#ifdef WIN32
	//windows program
	HANDLE m_hSendThread;
	HANDLE m_hRevThread;
#else
	// linux program
	pthread_t m_threads[2];
	int m_hSendThread;
	int m_hRevThread
#endif //end WIN32
};

#endif