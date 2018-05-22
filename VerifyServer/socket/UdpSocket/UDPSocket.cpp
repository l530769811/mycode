#include "StdAfx.h"
#include "UDPSocket.h"
#include "UdpSocketReceiveInterface.h"

CUDPSocket::CUDPSocket(CUdpSocketReceiveInterface *rev_interface)
{
#ifdef WIN32
	//windows program
	WORD wVersion = MAKEWORD(2, 2);
	WSAData wsaData;
	if (::WSAStartup(wVersion, &wsaData) != 0)
	{
		::WSACleanup();
		printf(("WSAStartup Fail\n"));
	}
	
	m_hSendThread = INVALID_HANDLE_VALUE;
	m_hRevThread = INVALID_HANDLE_VALUE;
	m_sockServer = 0;
#else
//linux program
	m_hRevThread = -1;
	m_hSendThread = -1;
	m_sockServer = -1;

#endif
	memset(&m_addrSend, 0, sizeof(m_addrSend));
	memset(&m_addrRevice, 0, sizeof(m_addrRevice));

	m_bStopUDPRevThread = true;
	m_bFinishUDPSend = false;
	m_pRevInterface = NULL;
	m_pRevInterface = rev_interface;
	
}

CUDPSocket::~CUDPSocket(void)
{
	Stop();
}

bool CUDPSocket::Start(unsigned short usPort)
{
	if (!_InitSocket(usPort))
	{
		return false;
	}

	if(_StartRecive(usPort) == false){
		return false;
	}

	return true;
}

bool CUDPSocket::_StartRecive(unsigned short usDesPort)
{
	if (m_sockServer < 0)
	{
		return false;
	}

	_WantStopThread();
#ifdef WIN32
	//windows program
	if (m_hRevThread != NULL && m_hRevThread != INVALID_HANDLE_VALUE)
	{
		::WaitForSingleObject(m_hRevThread, INFINITE);
		m_hRevThread = INVALID_HANDLE_VALUE;
	}

	m_addrRevice.sin_port=htons(usDesPort);
	
#else

#endif //WIN32



#ifdef WIN32
	//m_hSendThread = (HANDLE)_beginthreadex(NULL, 0, _SendThread,reinterpret_cast<void *>(this), 0, NULL);
	m_hRevThread = (HANDLE)_beginthreadex(NULL, 0, _RevThread,reinterpret_cast<void *>(this), 0, NULL);

#else
	//m_hSendThread = pthread_create(&(m_threads[0]),NULL,_SendThread,(void *)1);
	m_hRevThread = pthread_create(&(m_threads[1]),NULL,_RevThread,(void *)2);
#endif //WIN32
	if (m_hRevThread == NULL)
	{
		return false;
	}
	m_bStopUDPRevThread = false;
	return true;
}

bool CUDPSocket::Stop()
{
	bool bResult = true;

	_WantStopThread();
#ifdef WIN32
	//windows program
	if (m_hRevThread!=NULL && m_hRevThread!=INVALID_HANDLE_VALUE)
	{
		::WaitForSingleObject(m_hRevThread, INFINITE);
		m_hRevThread = INVALID_HANDLE_VALUE;
	}


	if(m_sockServer!=0 && closesocket(m_sockServer) == 0)
	{
		m_sockServer = 0;
	}
	else
	{
		bResult = false;
	}
	

	if(::WSACleanup() != 0)
	{
		bResult = false;
	}
#else
	//linux program
	//need to wait thread exit
#endif //endif WIN32

	

	return bResult;
}

void CUDPSocket::_WantStopThread()
{
	m_bStopUDPRevThread = true;
}

bool CUDPSocket::_IsWantStopThread()
{
	return m_bStopUDPRevThread;
}

bool CUDPSocket::_InitSocket(unsigned short usPort)
{
	if (m_sockServer > 0)
	{		
		return true;
	}

	std::string log;
	if ((m_sockServer = ::socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		return false;
	}
	
	m_addrSend.sin_family=AF_INET;  
	m_addrSend.sin_addr.s_addr=htonl(INADDR_BROADCAST);  
	//m_addrSend.sin_addr.s_addr = inet_addr("255.255.255.255");
	m_addrSend.sin_port=htons(usPort);  
	
	m_addrRevice.sin_family=AF_INET;  
	m_addrRevice.sin_addr.s_addr=htonl(INADDR_ANY);  
	m_addrRevice.sin_port=htons(usPort);  
	

	const int opt = 1;
	int nb = 0;
	if ((nb = ::setsockopt(m_sockServer, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt))) == -1)
	{
		printf(("setsockopt Fail\n"));
		
		::closesocket(m_sockServer);
		return false;
	}

	int nbind = -1;
	if((nbind = ::bind(m_sockServer, (sockaddr*)&m_addrRevice, sizeof(m_addrRevice))) == -1)
	{
		printf(("setsockopt1 fail\n"));
		::closesocket(m_sockServer);
		return false;
	}

	unsigned long TimeOut = 1000*5;
	if(::setsockopt(m_sockServer,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{
		printf(("setsockopt2 Fail\n"));

		::closesocket(m_sockServer);
		return false;
	}

	return true;
}

RESULT_THREAD CUDPSocket::_RevThread(void* pParam)
{
	CUDPSocket *pThis = (CUDPSocket*)pParam;
	
	while(1)
	{
		char buffer[1024*2] = {0};
		int len = sizeof(struct sockaddr_in);
		int ret=::recvfrom(
			 pThis->m_sockServer, 
			 buffer, 
			 256, 
			 0, 
			 (struct sockaddr*)&pThis->m_addrRevice,
			 &len); 

		if(ret<=0)  
		{ 

		}  
		else  
		{     
			if (pThis->m_pRevInterface != NULL)
			{
				char *ip = ::inet_ntoa(pThis->m_addrRevice.sin_addr);
				unsigned short port = pThis->m_addrRevice.sin_port;

				pThis->m_pRevInterface->rev_data((const unsigned char*)buffer, ret, ip, port);
			}
		}	

		if (pThis->_IsWantStopThread())
		{
			break;
		}
	
	}


#ifdef WIN32
	return 0;
#else
	pthread_exit(NULL);
#endif
}

int CUDPSocket::GlobalUnicodeToUtf8(const TCHAR *unicode, char *uft8, int len)
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

bool CUDPSocket::SendData(BYTE chData[], int nLen, const TCHAR * desIp, const unsigned short desPort)
{
	bool bResult = false;

	int n = sizeof(struct sockaddr_in);
	struct sockaddr_in addrSend = {0};
	addrSend.sin_family=AF_INET;
	char cip[32] = {0};
	GlobalUnicodeToUtf8(desIp, cip, 31);
	addrSend.sin_addr.s_addr=inet_addr(cip);  
	addrSend.sin_port=htons(desPort);

	int ret = sendto(m_sockServer, (char*)chData, nLen, 0, (sockaddr*)&addrSend, n);  
	if(ret<0)  
	{  
		printf("send error....\n"); 
		bResult = false;
	}  
	else  
	{         
		printf("ok \n");

		bResult = true;
	} 

	return bResult;
}

bool CUDPSocket::SendData(BYTE chData[], int nLen)
{
	bool bResult = false;

	int n = sizeof(struct sockaddr_in);
	int ret = sendto(m_sockServer, (char*)chData, nLen, 0, (sockaddr*)&m_addrSend, n);  
	::send(m_sockServer, (char*)chData, nLen, 0);
	if(ret<0)  
	{  
		printf("send error....\n"); 
		bResult = false;
	}  
	else  
	{         
		printf("ok \n");

		bResult = true;
	} 

	return bResult;
}

bool CUDPSocket::SendData(BYTE chData[], int nLen, const struct sockaddr_in &send_to)
{	
	bool bResult = false;

	int n = sizeof(struct sockaddr_in);
	int ret = ::sendto(m_sockServer, (char*)chData, nLen, 0, (sockaddr*)&send_to, n);  
	if(ret<0)  
	{  
		printf("send error....\n"); 
		bResult = false;
	}  
	else  
	{         
		printf("ok \n");
		//CLogFile file1;
		//unsigned long *pUULen = (unsigned long *)(chSendData +3+sizeof(unsigned long)*3);
		//unsigned long nUULen = *pUULen;
		//memcpy(chDebug, chSendData+3+sizeof(unsigned long)*4, nUULen);
		//
		//file1.WriteLog("send data UserName =  %s, len = %d", chDebug, nUULen);
		//unsigned long *pIIPLen = (unsigned long *)(chSendData +3+sizeof(unsigned long)*4 + nUULen);
		//unsigned long nIIPLen = *pIIPLen;
		//memset(chDebug, 0, nUULen+1);
		//memcpy(chDebug, chSendData +3+sizeof(unsigned long)*5 + nUULen, nIIPLen);
		//file1.WriteLog("send data IP =  %s, len = %d", chDebug, nIIPLen);
		bResult = true;
	} 

	return bResult;
}

void CUDPSocket::_TranData(char chData[], int len)
{

}