#include "stdafx.h"
#include "TcpServer.h"
#include <process.h>

#include "SocketRecevier.h"




#define IOCP_SI_EXIT					0x00000000
#define IOCP_SI_ACCEPT				0xFFFFFFF1
#define IOCP_SI_SEND					0xFFFFFFF2
#define IOCP_SI_DISCONNECT		0xFFFFFFF3
#define IOCP_SI_RS_GONO			0
#define IOCP_SI_RS_CONTINUE	1
#define IOCP_SI_RS_BREAK			2




int UnicodeToUtf8(const TCHAR *unicode, char *uft8, int len)
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

TCHAR* Utf8ToUnicode(const char *uft8, TCHAR *unicode, int len)
{
#ifdef _UNICODE

	int nLen = 0;
	nLen = MultiByteToWideChar(CP_UTF8, 0, (char*)uft8, -1, NULL, 0);
	if (len < nLen)
	{
		return NULL;
	}
	::memset(unicode, 0, len * sizeof(TCHAR));
	::MultiByteToWideChar(CP_UTF8, 0, (char*)uft8, -1, unicode, nLen);

#else
	strcpy(unicode, uft8);
#endif //_UNICODE
	return unicode;
}


CTcpServer::CTcpServer()
	: m_dwWorkerCount(MAX_THREAD)
	, m_dwAcceptSocketCount(MAX_ACCEPT)
	, m_dwConnID(0)
	, m_dwMaxConnections(10000)
	, m_dwFreeSocketPool(150)
	, m_dwFreeSocketHold(450)
	, m_dwFreeIOPool(300)
	, m_dwFreeIOHold(900)
	, m_dwMaxShutdownWaitTime(1000 * 15)
	, m_bRuning(FALSE)
	, m_pRecevier(NULL)
	, m_hCompletionPort(NULL)
	, m_pfnAcceptEx(NULL)
	, m_pfnGetAcceptExSockaddrs(NULL)
	, m_pfnDisconnectEx(NULL)
{
	::InitializeCriticalSection(&m_csFreeCxt);
	::InitializeCriticalSection(&m_csFreeOl);
	::InitializeCriticalSection(&m_csMapSocket);
}

CTcpServer::~CTcpServer()
{
	if (m_bRuning)
		Stop();
	::DeleteCriticalSection(&m_csFreeCxt);
	::DeleteCriticalSection(&m_csFreeOl);
	::DeleteCriticalSection(&m_csMapSocket);
}

BOOL CTcpServer::Start(CSocketRecevier *pReciver, const char* bind_address, const USHORT& port)
{
	// check is runing
	if (m_bRuning)
		return TRUE;

	m_pRecevier = pReciver;

	// Init Socket
	WSADATA wsaData;
	if ((WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		WSACleanup();
		
		return FALSE;
	}

	// Create listen SOCKET
	if ((m_sockListen = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) 
	{
		
		return FALSE;
	}


	// 填充地址信息结构
	struct sockaddr_in addr;
	ZeroMemory((char *)&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(bind_address);	//addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	// Bind our name to the socket
	if (::bind(m_sockListen, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		
		closesocket(m_sockListen);
		return FALSE;
	}

	// Set the socket to listen
	if (listen(m_sockListen, m_dwMaxConnections) == SOCKET_ERROR)
	{
		
		closesocket(m_sockListen);
		return FALSE;
	}

	GUID guid_accept = WSAID_ACCEPTEX;
	GUID guid_address = WSAID_GETACCEPTEXSOCKADDRS;
	GUID guid_disconect = WSAID_DISCONNECTEX;
	m_pfnAcceptEx = (LPFN_ACCEPTEX)GetExtensionFuncPtr(m_sockListen, guid_accept);
	m_pfnGetAcceptExSockaddrs = (LPFN_GETACCEPTEXSOCKADDRS)GetExtensionFuncPtr(m_sockListen, guid_address);
	m_pfnDisconnectEx = (LPFN_DISCONNECTEX)GetExtensionFuncPtr(m_sockListen, guid_disconect);
	ASSERT(m_pfnAcceptEx);
	ASSERT(m_pfnGetAcceptExSockaddrs);
	ASSERT(m_pfnDisconnectEx);

	// Create the copletion port
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
		NULL, 0, 0);
	if (m_hCompletionPort == NULL)
	{
		
		closesocket(m_sockListen);
		return FALSE;
	}

	// Determine how many processors are on the system.
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	m_dwWorkerCount = info.dwNumberOfProcessors * 2 + 2;

	// Create worker thread
	for (UINT i = 0; i < m_dwWorkerCount; i++) 
	{
		HANDLE hThread =  (HANDLE)_beginthreadex(NULL, 0, WorkerThreadProc, reinterpret_cast<void *>(this), 0, NULL);
		if (hThread) 
		{
			m_vecWorkThread.push_back(hThread);
		}
		else
		{
			
			CloseHandle(hThread);
			return FALSE;
		}
	}

	// Post accept
	if (CreateIoCompletionPort((HANDLE)m_sockListen, m_hCompletionPort, m_sockListen, 0))
	{
		for (UINT i = 0; i < m_dwAcceptSocketCount; i++)
			::PostQueuedCompletionStatus(m_hCompletionPort, IOCP_SI_ACCEPT, 0, NULL);
	}

	m_bRuning = TRUE;

	return TRUE;
}

BOOL CTcpServer::Stop()
{
	if (!m_bRuning)
		return TRUE;

	// close listen socket
	if (m_sockListen != INVALID_SOCKET)
	{
		ManualCloseSocket(m_sockListen);
		m_sockListen = INVALID_SOCKET;
	}

	// close all connect socket
	MPSOCKETCONTEXT::iterator iter = m_mpClientSocket.begin();
	while (iter != m_mpClientSocket.end())
	{
		PSOCKET_CONTEXT pSock = iter->second;
		if (pSock)
		{
			::PostQueuedCompletionStatus(m_hCompletionPort, IOCP_SI_DISCONNECT, iter->first, NULL);
		}
		iter++;
	}

	// wait for client socket close
	DWORD dwWait = 0;
	DWORD dwOrig = ::timeGetTime();
	while (m_mpClientSocket.size() > 0 && dwWait < m_dwMaxShutdownWaitTime)
	{
		dwWait = ::timeGetTime() - dwOrig;
	}

	// wait for work thread exit
	UINT nCount = m_vecWorkThread.size();
	for (UINT i = 0; i < nCount; i++)
		::PostQueuedCompletionStatus(m_hCompletionPort, IOCP_SI_EXIT, 0, NULL);

	UINT remain = nCount;
	UINT index = 0;
	while (remain > 0)
	{
		UINT wait = min(remain, MAXIMUM_WAIT_OBJECTS);
		HANDLE* pHandles = (HANDLE*)_alloca(sizeof(HANDLE) * wait);
		for (UINT i = 0; i < wait; i++)
			pHandles[i] = m_vecWorkThread[i + index];

		WaitForMultipleObjects((DWORD)wait, pHandles, TRUE, INFINITE);

		for (UINT i = 0; i < wait; i++)
			CloseHandle(pHandles[i]);

		remain -= wait;
		index += wait;
	}
	m_vecWorkThread.clear();

	// delete client context
	{
		MPSOCKETCONTEXT::iterator iter = m_mpClientSocket.begin();
		while (iter != m_mpClientSocket.end())
		{
			PSOCKET_CONTEXT pSock = iter->second;
			ManualCloseSocket(pSock->sock, 1);
			delete pSock;
			pSock = NULL;
		}
		m_mpClientSocket.clear();
	}

	// delete free socket context
	{
		::EnterCriticalSection(&m_csFreeOl);
		LSSOCKETCONTEXT::iterator iter = m_lsFreeSocketContext.begin();
		while (iter != m_lsFreeSocketContext.end())
		{
			PSOCKET_CONTEXT pSock = m_lsFreeSocketContext.front();
			delete pSock;
			
			
			pSock = NULL;
			iter = m_lsFreeSocketContext.erase(iter);
		}
		::LeaveCriticalSection(&m_csFreeOl);
	}

	// delete free io context
	{
		::EnterCriticalSection(&m_csFreeCxt);
		LSIOCONTEXT::iterator iter = m_lsFreeIOContext.begin();
		while (iter != m_lsFreeIOContext.end())
		{
			PIO_CONTEXT pIO = m_lsFreeIOContext.front();
			delete pIO;
			pIO = NULL;
			iter = m_lsFreeIOContext.erase(iter);
		}
		::LeaveCriticalSection(&m_csFreeCxt);
	}

	// close completeport
	if (m_hCompletionPort != NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort = NULL;
	}

	// reset data
	m_dwConnID = 0;
	m_pfnAcceptEx = NULL;
	m_pfnGetAcceptExSockaddrs = NULL;
	m_pfnDisconnectEx = NULL;
	m_bRuning = FALSE;

	return TRUE;
}

BOOL CTcpServer::_send(const DWORD& nID, const BYTE* pBuf, const int& nLen, BOOL bPostQueued /* = FALSE */)
{
	ASSERT(pBuf && nLen > 0);

	PSOCKET_CONTEXT pSock = FindSocketContext(nID);
	if (pSock == NULL)
		return FALSE;

	int result = 0;
	int remain = nLen;
	while (remain > 0)
	{
		int bufferSize = min(remain, MAX_BUFFER);
		PIO_CONTEXT pIO = GetFreeIOContext(nLen);
		memcpy(pIO->wsaBuf.buf, pBuf, nLen);
		pIO->wsaBuf.len = nLen;
		pIO->so = SO_WRITE;
		pIO->sockAccept = pSock->sock;
		if (bPostQueued)
			::PostQueuedCompletionStatus(m_hCompletionPort, 0, (ULONG_PTR)pIO, &pIO->ol);
		else
			result = PostSend(pIO);
		if (result != 0)
		{
			AddFreeIOContext(pIO); 
			break;
		}
		//TRACE(_T("===========\n%s\n"), pBuf);
		remain -= bufferSize;
		pBuf += bufferSize;
	}
	
	return !result;
}

BOOL CTcpServer::Send(const DWORD& nID, const BYTE* pBuf, const int& nLen, BOOL bPostQueued /* = FALSE */)
{
	BYTE pSendBuf[1024] = {0};
	int nNum = PrepareSendBuf(pSendBuf, pBuf, nLen, 50);
	return this->_send(nID, pSendBuf, nNum);
}

BOOL CTcpServer::SendEx(const DWORD& nID, const mystring& csBuf)
{
	//OutputDebugString(csBuf);
	//OutputDebugString(_T("\n"));
	BOOL bRet = FALSE;

	PSOCKET_CONTEXT pSock = FindSocketContext(nID);
	if (pSock == NULL)
		return bRet;
	
	int nLen = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)(LPCWSTR)csBuf.c_str(), -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
		return FALSE;
	char* str = new char[1024];
	WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)(LPCWSTR)csBuf.c_str(), -1, str, sizeof(char *)*1024, NULL, NULL);
	nLen = strlen(str);
	BYTE * pTmp = new BYTE[nLen+1];
	memcpy(pTmp, str, nLen);
	pTmp[nLen] = NULL;
	BYTE * pBuf = new BYTE[nLen+50];
	int nNum = PrepareSendBuf(pBuf, pTmp, nLen, 50);


	bRet = _send(nID, pBuf, nNum);

	delete str;
	delete [] pTmp;
	delete [] pBuf;

	return bRet;
}

BOOL CTcpServer::Disconnect(const DWORD& nID, BOOL bForce /* = TRUE */)
{
	BOOL bOK = FALSE;
	PSOCKET_CONTEXT pSock = FindSocketContext(nID);
	if (pSock)
	{
		if (bForce)
			bOK = ::PostQueuedCompletionStatus(m_hCompletionPort, IOCP_SI_DISCONNECT, nID, NULL);
		else
			bOK = m_pfnDisconnectEx(pSock->sock, NULL, 0, 0);
	}

	return bOK;
}


BOOL CTcpServer::GetClientAddress(DWORD dwConnID, LPTSTR lpszAddress, int& iAddressLen, USHORT& usPort)
{
	PSOCKET_CONTEXT pSock = FindSocketContext(dwConnID);
	if (pSock)
	{
		BOOL isOK		= TRUE;
		ADDRESS_FAMILY usFamily = pSock->addr.sin_family;
		usPort = ntohs(pSock->addr.sin_port);
		char* lpszIP	= inet_ntoa(pSock->addr.sin_addr);
		int iIPLen		= (int)strlen(lpszIP);

		if(iIPLen > 0)
			++iIPLen;

		if(iIPLen > 0 && iAddressLen >= iIPLen)
		{
			TCHAR szIP[256] = {0};
			lstrcpy(lpszAddress, Utf8ToUnicode(lpszIP,szIP,255));
		}
		else
			isOK = FALSE;

		iAddressLen = iIPLen;
		return isOK;
	}

	return FALSE;
}



//////////////////////////////////////////////////////////////////////////
// Private method
unsigned int __stdcall CTcpServer::WorkerThreadProc(LPVOID lpParam)
{
	CTcpServer* pThis = reinterpret_cast<CTcpServer *>(lpParam);
	ASSERT(pThis);
	

	while (TRUE)
	{
		DWORD dwBytes = 0, dwErrorCode = 0;
		WSAOVERLAPPED* pOverlapped = NULL;
		SOCKET_CONTEXT* pSock = NULL;

		BOOL result = ::GetQueuedCompletionStatus(pThis->m_hCompletionPort, 
			&dwBytes, 
			(PULONG_PTR)&pSock,
			&pOverlapped,
			INFINITE);
		if (pOverlapped == NULL)
		{
			if (dwBytes == IOCP_SI_ACCEPT)
			{
				SOCKET sockClient = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
				PIO_CONTEXT pOL = pThis->GetFreeIOContext();
				if (pThis->PostAccept(pThis->m_pfnAcceptEx, pThis->m_sockListen, sockClient, pOL) != 0)
				{
					pThis->ManualCloseSocket(sockClient);
					pThis->AddFreeIOContext(pOL);
				}
				
				continue;
			}
			else if (dwBytes == IOCP_SI_DISCONNECT)
			{
				DWORD dwConnID = (DWORD)pSock;
				PSOCKET_CONTEXT pSock = pThis->FindSocketContext(dwConnID);
				if (pSock)
					pThis->AddFreeSocketContext(dwConnID, SCF_CLOSE);
				continue;
			}
			else if (dwBytes == IOCP_SI_EXIT && pSock == NULL)
			{
				break;
			}
		}

		PIO_CONTEXT pOL = CONTAINING_RECORD(pOverlapped, IO_CONTEXT, ol);
		DWORD connID = pOL->so != SO_ACCEPT ? pSock->connID : 0;

		if (!result)
		{
			DWORD dwFlags = 0;
			DWORD dwSysCode = ::GetLastError();
			SOCKET sock = pOL->so != SO_ACCEPT ? pOL->sockAccept : pThis->m_sockListen;
			result = ::WSAGetOverlappedResult(sock, &pOL->ol, &dwBytes, FALSE, &dwFlags);
			if (!result)
			{
				dwErrorCode = ::WSAGetLastError();
				
			}
		}

		if (dwErrorCode != 0)
		{
			if (dwErrorCode == WSA_IO_PENDING)
			{
				continue;
			}

			if (pOL->so != SO_ACCEPT)
			{
				if (dwErrorCode != WSAENOTSOCK && dwErrorCode != ERROR_OPERATION_ABORTED)
				{
					pThis->AddFreeSocketContext(connID, SCF_ERROR, pOL->so, dwErrorCode);
				}
			}
			else
			{
				pThis->ManualCloseSocket(pOL->sockAccept);
				PostQueuedCompletionStatus(pThis->m_hCompletionPort, IOCP_SI_ACCEPT, 0, NULL);
			}
			pThis->AddFreeIOContext(pOL);
			continue;
		}
		if (dwBytes == 0 && pOL->so != SO_ACCEPT)
		{
			pThis->AddFreeSocketContext(connID, SCF_CLOSE);
			pThis->AddFreeIOContext(pOL);
			continue;
		}

		switch (pOL->so)
		{
		case SO_ACCEPT:
			{
				pThis->ProcessAccept(pThis->m_sockListen, pOL);
				break;
			}
		case SO_READ:
			{
				pThis->ProcessReceive(connID, pSock, pOL, dwBytes);
				break;
			}
		case SO_WRITE:
			{
				pThis->ProcessSend(connID, pSock, pOL, dwBytes);
				break;
			}
		default:
			break;
		}
	}

	return 0;
}


int CTcpServer::ProcessAccept(SOCKET sockListen, PIO_CONTEXT pIO)
{
	::PostQueuedCompletionStatus(m_hCompletionPort, IOCP_SI_ACCEPT, 0, NULL);

	int nLocalAddr, nRemoteAddr;
	SOCKADDR* pLocalAddr, *pRemoteAddr;
	m_pfnGetAcceptExSockaddrs(
		pIO->wsaBuf.buf, 
		0, 
		sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, 
		(SOCKADDR**)&pLocalAddr, 
		&nLocalAddr, 
		(SOCKADDR**)&pRemoteAddr, 
		&nRemoteAddr);

	SOCKET socket = pIO->sockAccept;
	DWORD connID = ::InterlockedIncrement(&m_dwConnID);
	if (connID == 0)
		connID = ::InterlockedIncrement(&m_dwConnID);
	PSOCKET_CONTEXT pSock = GetFreeSocketContext(connID, socket);

	memcpy(&pSock->addr, pRemoteAddr, sizeof(SOCKADDR_IN));
	AddClientSocket(connID, pSock);

	if (::setsockopt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (CHAR*)&sockListen, sizeof(SOCKET)))
	{
		;
	}

	DWORD dwBytes;

	MyTcpKeepAlive klive = {0};
	//window自带KeepAlive结构体，头文件包含MSTcpIP.h;
	//tcp_keepalive klive = {0};
	klive.onoff = 1;
	klive.keepalivetime = 1000 * 30;
	klive.keepaliveinterval = 1000 * 1;
	if (::WSAIoctl(socket, SIO_KEEPALIVE_VALS, (LPVOID)&klive, sizeof(MyTcpKeepAlive), NULL, 0, &dwBytes, NULL, NULL))
	{
		;
	}

	if (CreateIoCompletionPort((HANDLE)socket, m_hCompletionPort, (ULONG_PTR)pSock, 0) == NULL)
	{
		AddFreeSocketContext(connID, SCF_NULL);
		AddFreeIOContext(pIO);
		return -1;
	}

	if (PostReceive(pIO) != 0)
		AddFreeIOContext(pIO);

	return 0;
}

int CTcpServer::ProcessSend(DWORD dwConnID, PSOCKET_CONTEXT pSock, PIO_CONTEXT pIO, DWORD dwIoSize /* = 0 */)
{
	int result = 0;
	//pIO->writeBuf.Delete(dwIoSize);
	//if (pIO->writeBuf.GetBufferLen() == 0)
	//{
	//	pIO->writeBuf.ClearBuffer();
	//}
	//else
	//{
	//	pIO->wsaBuf.buf = (char*)pIO->writeBuf.GetBuffer();
	//	pIO->wsaBuf.len = pIO->writeBuf.GetBufferLen();
	//	result = PostSend(pIO);
	//	if (result != 0)
	//		AddFreeIOContext(pIO);
	//	else
	//		pIO->writeBuf.Delete(pIO->writeBuf.GetBufferLen());

	//	pIO->wsaBuf.len = MAX_BUFFER;
	//	result = PostReceive(pIO);
	//	if (result != 0)
	//	{
	//		AddFreeIOContext(pIO);
	//	}
	//}

	AddFreeIOContext(pIO);
	
	return result;
}

int CTcpServer::ProcessReceive(DWORD dwConnID, PSOCKET_CONTEXT pSock, PIO_CONTEXT pIO, DWORD dwIoSize /* = 0 */)
{
	pIO->wsaBuf.len = dwIoSize;
	pIO->connID = pSock->connID;
	SaveData(pIO, pIO->wsaBuf.buf, dwIoSize);
	//TRACE(_T("Receice:%s\n"), pIO->wsaBuf.buf);

	pIO->wsaBuf.len = MAX_BUFFER;
	int result = PostReceive(pIO);
	if (result != 0)
	{
		AddFreeSocketContext(dwConnID, SCF_ERROR, SO_READ, ERROR_FUNCTION_FAILED);
		AddFreeIOContext(pIO);
	}

	return result;
}

PSOCKET_CONTEXT CTcpServer::GetFreeSocketContext(DWORD dwConnID, SOCKET sockClient)
{
	SOCKET_CONTEXT* pSock = NULL;
	::EnterCriticalSection(&m_csFreeOl);
	if (m_lsFreeSocketContext.size() > 0)
	{
		pSock = m_lsFreeSocketContext.front();
		m_lsFreeSocketContext.pop_front();
	}
	::LeaveCriticalSection(&m_csFreeOl);

	if (pSock == NULL)
		pSock = new SOCKET_CONTEXT;

	pSock->connID = dwConnID;
	pSock->sock = sockClient;

	return pSock;
}

PIO_CONTEXT CTcpServer::GetFreeIOContext(int nLen /* = 0 */)
{
	PIO_CONTEXT pIO = NULL;
	::EnterCriticalSection(&m_csFreeCxt);
	if (m_lsFreeIOContext.size() > 0)
	{
		
		pIO = m_lsFreeIOContext.front();
		m_lsFreeIOContext.pop_front();
		             
	}
	::LeaveCriticalSection(&m_csFreeCxt);   
	if (pIO == NULL)
		pIO = new IO_CONTEXT;

	return pIO;
}

void CTcpServer::AddFreeSocketContext(DWORD dwConnID, ESocketCloseFlag eFlag /* = SCF_NULL */, ESocketOperation eSO /* = SO_NULL */, DWORD dwErrorCode /* = 0 */)
{
	PSOCKET_CONTEXT pSock = FindSocketContext(dwConnID);
	if (pSock)
	{
		::EnterCriticalSection(&m_csMapSocket);
		ManualCloseSocket(pSock->sock);
		m_mpClientSocket.erase(dwConnID);
		::LeaveCriticalSection(&m_csMapSocket);
		
		::EnterCriticalSection(&m_csFreeOl);
		m_lsFreeSocketContext.push_back(pSock);
		if (m_lsFreeSocketContext.size() > m_dwFreeSocketHold)
		{
			while (m_lsFreeSocketContext.size() > m_dwFreeSocketPool)
			{
				PSOCKET_CONTEXT pSock = m_lsFreeSocketContext.front();
				m_lsFreeSocketContext.pop_front();
				delete pSock;
				pSock = NULL;
			}
		}
		::LeaveCriticalSection(&m_csFreeOl);
/*
		cJSON *p = CreateCommonRequestJson();
		
		cJSON *pContent = NULL;
		cJSON_AddItemToObject(p,
			JSON_DOWNLINE_REQUEST_CONTENT, 
			pContent=cJSON_CreateObject());

		cJSON_AddStringToObject(pContent, 
			JSON_DOWNLINE_REQUEST_TYPE_KEY, 
			JSON_DOWNLINE_REQUEST_TYPE_VALUE);

		TCHAR *out = NULL;
		out = cJSON_Print(p);
		
		if (m_pRecevier != NULL)
		{
			m_pRecevier->Recevie(dwConnID, (BYTE*)out, sizeof(TCHAR)*_tcslen(out));
		}

		free(out);

		DeleteCommonRequestJson(p);	*/	
	}
}

void CTcpServer::AddFreeIOContext(PIO_CONTEXT pIO)
{
	::EnterCriticalSection(&m_csFreeCxt);
	m_lsFreeIOContext.push_back(pIO);
	if (m_lsFreeIOContext.size() > m_dwFreeIOHold)
	{
		while (m_lsFreeIOContext.size() > m_dwFreeIOPool)
		{
			PIO_CONTEXT pIO = m_lsFreeIOContext.front();
			m_lsFreeIOContext.pop_front();
			delete pIO;
			pIO = NULL;
		}
	}
	::LeaveCriticalSection(&m_csFreeCxt);
}

void CTcpServer::AddClientSocket(DWORD dwConnID, PSOCKET_CONTEXT pSock)
{
	if (pSock == NULL)
		return ;
	::EnterCriticalSection(&m_csMapSocket);
	m_mpClientSocket[dwConnID] = pSock;
	::LeaveCriticalSection(&m_csMapSocket);
}

void CTcpServer::RemoveClientSocket(PSOCKET_CONTEXT pSock, ESocketCloseFlag eFlag /* = SCF_NULL */, ESocketOperation eSO /* = SO_NULL */, DWORD dwErrorCode /* = 0 */, int nShutdownFlag /* = SD_SEND */)
{
	if (pSock == NULL)
		return ;

	ManualCloseSocket(pSock->sock, nShutdownFlag);
}

PSOCKET_CONTEXT CTcpServer::FindSocketContext(DWORD dwConnID)
{
	SOCKET_CONTEXT* pSock = NULL;

	::EnterCriticalSection(&m_csMapSocket);
	MPSOCKETCONTEXT::iterator iter = m_mpClientSocket.find(dwConnID);
	if (iter != m_mpClientSocket.end())
		pSock = iter->second;
	::LeaveCriticalSection(&m_csMapSocket);

	return pSock;
}

PVOID CTcpServer::GetExtensionFuncPtr(SOCKET sock, GUID guid)
{
	DWORD dwByte;
	PVOID pfn = NULL;
	WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &pfn, sizeof(pfn), &dwByte, NULL, NULL);
	return pfn;
}

int CTcpServer::ManualCloseSocket(SOCKET sock, int nShutDownFlag /* = 0xFF */, BOOL bGraceful /* = TRUE */, BOOL bReuseAddress /* =FALSE */)
{
	int result = 0;
	if (!bGraceful)
	{
		linger ln = {1, 0};
		result = setsockopt(sock, SOL_SOCKET, SO_LINGER, (CHAR*)&ln, sizeof(linger));
	}

	if (bReuseAddress)
		result = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (CHAR*)&bReuseAddress, sizeof(BOOL));

	if (nShutDownFlag != 0xFF)
		shutdown(sock, nShutDownFlag);

	result = closesocket(sock);

	return result;
}

int CTcpServer::PostAccept(LPFN_ACCEPTEX pfnAcceptEx, SOCKET sockListen, SOCKET sockClient, PIO_CONTEXT pIO)
{
	int result = 0;
	pIO->so = SO_ACCEPT;
	pIO->sockAccept = sockClient;

	if (!pfnAcceptEx(
		sockListen, 
		pIO->sockAccept, 
		pIO->wsaBuf.buf, 
		0, 
		sizeof(SOCKADDR_IN) + 16, 
		sizeof(SOCKADDR_IN) + 16, 
		NULL, 
		&pIO->ol))
	{
		result = ::WSAGetLastError();
		if (result == WSA_IO_PENDING)
			return 0;
	}

	return result;
}

int CTcpServer::PostSend(PIO_CONTEXT pIO)
{
	int result = 0;
	DWORD dwBytes = 0;
	pIO->so = SO_WRITE;

	if ((result=::WSASend(
		pIO->sockAccept, 
		&pIO->wsaBuf, 
		1, 
		&dwBytes, 
		0, 
		&pIO->ol, 
		NULL)) == SOCKET_ERROR)
	{
		result = ::WSAGetLastError();
		if (result == WSA_IO_PENDING)
			result = 0;
	}

	return result;
}

int CTcpServer::PostReceive(PIO_CONTEXT pIO)
{
	int result = 0;
	DWORD dwBytes, dwFlag = 0;
	pIO->so = SO_READ;
	pIO->wsaBuf.len = MAX_BUFFER;
	if (::WSARecv(pIO->sockAccept, &pIO->wsaBuf, 1, &dwBytes, &dwFlag, &pIO->ol, NULL) == SOCKET_ERROR)
	{
		result = ::WSAGetLastError();
		if (result == WSA_IO_PENDING)
			result = 0;
	}

	return result;
}

int CTcpServer::GetPacketLen(BYTE * pBuf, int nLen)
{
	if (nLen <= 10)
		return 0xffffffff;
#ifndef REV_UNICODE

	if (pBuf[0] == 'S' && pBuf[1] == ':')
	{
		DWORD * pLen = (DWORD *)(&pBuf[6]);
		if (*pLen >= 15)
		{
			return *pLen;
		}
	}
#else
	TCHAR *p = (TCHAR*)pBuf;
	DWORD flaglen = (sizeof(TCHAR)*2 +
		sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD) +
		sizeof(TCHAR));
	if (p[0] == 'S' && p[1] == ':')
	{
		DWORD * pLen = (DWORD *)(&pBuf[sizeof(TCHAR)*2 + sizeof(DWORD)]);
		if (*pLen >= flaglen)
		{
			return *pLen;
		}
	}

#endif 
	return 0xffffffff;
}

int CTcpServer::PrepareSendBuf(BYTE * pBufDes, const BYTE * pBufSrc, const int &nLenSrc, DWORD ParamSrc)
{
	//发送数据格式
	//字节数:  2    4    4    x     4     1
	//内容  : 'S:'+参数+长度+数据+检验码+'E'
#ifndef REV_UNICODE
	pBufDes[0] = 'S';
	pBufDes[1] = ':';
	DWORD *wd = (DWORD *)(&pBufDes[2]);
	*wd = ParamSrc;
	wd = (DWORD *)(&pBufDes[6]);
	*wd = nLenSrc + 15;		//包的总长度
	int nIndex = sizeof(DWORD) * 2 + 2;
	memcpy(pBufDes + nIndex, pBufSrc, nLenSrc);
	wd = (DWORD *)(pBufDes + nIndex + nLenSrc);
	DWORD nSum = 0;
	for(int i = 0; i!= nIndex + nLenSrc; ++i)
	{
		nSum += pBufDes[i];
	}
	*wd = nSum;
	int nLast = nIndex + nLenSrc + sizeof(DWORD);
	pBufDes[nLast] = 'E';
#else
	TCHAR *pBuf = (TCHAR*)pBufDes;
	pBuf[0] = 'S';
	pBuf[1] = ':';
	DWORD *wd = (DWORD *)(&pBufDes[2*sizeof(TCHAR)]);
	*wd = ParamSrc;
	wd = (DWORD *)(&pBufDes[sizeof(TCHAR)*2 + sizeof(DWORD)]);
	*wd = nLenSrc + sizeof(TCHAR)*2 + sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD) + sizeof(TCHAR);		//包的总长度
	int nIndex = sizeof(DWORD)*2 + sizeof(TCHAR)*2;
	memcpy(pBufDes + nIndex, pBufSrc, nLenSrc);
	wd = (DWORD *)(pBufDes + nIndex + nLenSrc);
	DWORD nSum = 0;
	for(int i = 0; i!= (nIndex + nLenSrc)/sizeof(TCHAR); ++i)
	{
		nSum += pBuf[i];
	}
	*wd = nSum;
	int nLast = nIndex + nLenSrc + sizeof(DWORD);
	pBuf[nLast] = 'E';

#endif //REV_UNICODE
	return nLast + 1;
}

void CTcpServer::SaveData(PIO_CONTEXT pIO, const char* pBuf, const int nLen)
{
	//获取包的长度;
	if (pIO->tmpHeadBuf.nLen >0 )	//如果有上一次保留的头;
	{
		RCV_BUF tmp;
		tmp.nLen = pIO->tmpHeadBuf.nLen + nLen;
		tmp.pBuf = new char[tmp.nLen];
		memcpy(tmp.pBuf, pIO->tmpHeadBuf.pBuf, //将上次的头拷贝至前面;
			pIO->tmpHeadBuf.nLen);
		memcpy(tmp.pBuf + pIO->tmpHeadBuf.nLen,//拷贝本次数据;
			pBuf, nLen);
		delete [] pIO->tmpHeadBuf.pBuf;
		pIO->tmpHeadBuf.pBuf = NULL;
		pIO->tmpHeadBuf.nLen = 0;
		SaveData(pIO, tmp.pBuf, tmp.nLen);
		delete [] tmp.pBuf;
		tmp.pBuf = NULL;
		return ;
	}
	//先看上次还差多少数据;
	int nDesLeave = pIO->tmpBuf.nDesLen - pIO->tmpBuf.nCurLen;
	int nSrcLeave = nLen;
	if (nDesLeave == 0)
	{
		//先判断包长度;
		if (nLen >10)
		{
			int nDesLen = GetPacketLen((BYTE *)pBuf, nLen);
			if (0xffffffff != nDesLen)
			{
				if (pIO->tmpBuf.pBuf)
					delete [] pIO->tmpBuf.pBuf;
				pIO->tmpBuf.pBuf = new char[nDesLen];
				pIO->tmpBuf.nCurLen = 0;
				pIO->tmpBuf.nDesLen = nDesLen;
				SaveData(pIO, pBuf, nLen);
			}
			else
			{
				//说明刚收到包头之前的一点点数据;
				;
			}
		}
		else//小于10的情况不包含包头信息;
		{
			//这里就应该累计下一次的数据;
			pIO->tmpHeadBuf.nLen = nLen;
			pIO->tmpHeadBuf.pBuf = new char[nLen];
			memcpy(pIO->tmpHeadBuf.pBuf, pBuf, nLen);
		}

	}else if (nDesLeave > 0)
	{
		if (nLen >= nDesLeave)
		{
			memcpy(pIO->tmpBuf.pBuf + pIO->tmpBuf.nCurLen
				, pBuf, nDesLeave);//临时缓冲一个完整的包够数了;
			pIO->tmpBuf.nCurLen += nDesLeave;
			nSrcLeave -= nDesLeave;

			//存包;
			RCV_BUF buf;
			buf.pBuf = pIO->tmpBuf.pBuf;
			buf.nLen = pIO->tmpBuf.nDesLen;
			//pCxt->pLock->Lock();
			//int num = pCxt->qRcvBuf.size();
			//if (num > pCxt->nMaxQSize)
			//{
			//	//TRACE(_T("%d 接收缓冲满！删除第一个包！\n"),netTrm.nID);
			//	RCV_BUF * popBuf = pCxt->qRcvBuf.front();
			//	delete [] popBuf->pBuf;
			//	//netTrm.qRcvBuf.pop();
			//	pCxt->qRcvBuf.pop();
			//}
			//pCxt->qRcvBuf.push(&buf);
			//pCxt->pLock->Unlock();
#ifndef REV_UNICODE
			int nLen = buf.nLen - 15;
			BYTE *tmp = new BYTE[nLen+1];
			memcpy(tmp, buf.pBuf+10, buf.nLen-15);
			tmp[nLen] = NULL;
			nLen = MultiByteToWideChar(CP_UTF8, 0, (char*)tmp, -1, NULL, 0);
#else
			DWORD flaglen = (sizeof(TCHAR)*2 +
				sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD) +
				sizeof(TCHAR));
			int nLen = buf.nLen - flaglen;
			BYTE *tmp = new BYTE[nLen+1];
			memcpy(tmp, buf.pBuf+(sizeof(TCHAR)*2 +
				sizeof(DWORD) + sizeof(DWORD)), buf.nLen-flaglen);
			tmp[nLen] = NULL;
#endif	//REV_UNICODE		
			if (nLen > 0)
			{
				//TCHAR *pUnicodeBuf = new TCHAR[nLen+1];
				//Utf8ToUnicode((char*)tmp, pUnicodeBuf, nLen);

				//if (m_pRecevier != NULL)
				//{
				//	m_pRecevier->Recevie(pIO->connID, (BYTE*)pUnicodeBuf, _tcslen(pUnicodeBuf)*sizeof(TCHAR));
				//}

				//delete[] pUnicodeBuf;

				if (m_pRecevier != NULL)
				{
					m_pRecevier->Recevie(pIO->connID, (BYTE*)tmp, nLen);
				}

				delete [] buf.pBuf;
			}
			delete [] tmp;

			//完成;
			pIO->tmpBuf.pBuf = NULL;
			pIO->tmpBuf.nCurLen = 0;
			pIO->tmpBuf.nDesLen = 0;

			if (nSrcLeave >0) //一个新的包;
				SaveData(pIO, pBuf + nDesLeave, nSrcLeave);
		}
		else if (nLen < nDesLeave)
		{
			memcpy(pIO->tmpBuf.pBuf + pIO->tmpBuf.nCurLen, pBuf, nLen);
			pIO->tmpBuf.nCurLen += nLen;
		}
	}
}