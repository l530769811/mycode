#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <WinSock2.h>
#include <MSWSock.h>
#include <MSTcpIP.h>
#include <MMSystem.h>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <Windows.h>

#include "tcpsocket_define.h"
#include "Buffer.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm")



#define MAX_THREAD			32
#define MAX_BUFFER			1024 * 1
#define MAX_ACCEPT			256
#define ADDRESSLEN			(sizeof(SOCKADDR_IN) + 16)

enum ESocketCloseFlag
{
	SCF_NULL = 0,
	SCF_CLOSE,
	SCF_ERROR
};

enum ESocketOperation
{
	SO_NULL = 0,
	SO_INIT,
	SO_ACCEPT,
	SO_CONNECT,
	SO_READ,
	SO_WRITE
};

struct MyTcpKeepAlive   
{   
	DWORD onoff;   
	DWORD keepalivetime;   
	DWORD keepaliveinterval;   
};

struct RCV_BUF
{
	RCV_BUF() : pBuf(NULL), nLen(0){};
	char * pBuf;
	int nLen;
};
struct RCV_TMPBUF
{
	RCV_TMPBUF()
		: pBuf(NULL)
		, nDesLen(0)
		, nCurLen(0){};
	char * pBuf;
	int nDesLen;
	int nCurLen;
};

typedef struct _SOCKET_CONTEXT
{
public:
	_SOCKET_CONTEXT() { clean(); }
	~_SOCKET_CONTEXT() { clean(); }
	void clean()
	{
		connID = 0;
		sock = INVALID_SOCKET;
		memset(&addr, 0, sizeof(SOCKADDR_IN));
		addr.sin_addr.S_un.S_addr = INADDR_ANY;
		addr.sin_port = htons(0);
		addr.sin_family = AF_INET;
	}

	DWORD connID;
	SOCKET sock;
	SOCKADDR_IN addr;
}SOCKET_CONTEXT, *PSOCKET_CONTEXT;

typedef struct _IO_CONTEXT
{
public:
	_IO_CONTEXT() { clean(); }
	~_IO_CONTEXT() { clean(); }
	void clean()
	{
		connID = 0;
		sockAccept = INVALID_SOCKET;
		memset(&ol, 0, sizeof(WSAOVERLAPPED));
		memset(szBuf, 0, MAX_BUFFER);
		memset(&tmpBuf, 0, sizeof(RCV_TMPBUF));
		memset(&tmpHeadBuf, 0, sizeof(RCV_BUF));
		wsaBuf.buf = szBuf;
		wsaBuf.len = MAX_BUFFER;
		so = SO_NULL;
	}
	DWORD connID;
	SOCKET					sockAccept;
	WSAOVERLAPPED		ol;
	WSABUF					wsaBuf;
	CHAR						szBuf[MAX_BUFFER];
	CBuffer						writeBuf;
	RCV_TMPBUF tmpBuf;
	RCV_BUF tmpHeadBuf;
	ESocketOperation		so;
}IO_CONTEXT, *PIO_CONTEXT;


int UnicodeToUtf8(const TCHAR *unicode, char *uft8, int len);
TCHAR* Utf8ToUnicode( const char *uft8, TCHAR *unicode, int len);

class CSocketRecevier;
class _TCPSOCKET_IMPORT_EXPORT_DLL_ CTcpServer
{
public:
	CTcpServer();
	virtual ~CTcpServer();

	BOOL Start(CSocketRecevier *pReciver, const char* bind_address, const USHORT& port);

	BOOL Stop();
	BOOL Send(const DWORD& nID, const BYTE* pBuf, const int& nLen, BOOL bPostQueued = FALSE);
	BOOL SendEx(const DWORD& nID, const mystring& csBuf);
	BOOL Disconnect(const DWORD& nID, BOOL bForce = TRUE);
	BOOL GetClientAddress(DWORD dwConnID, LPTSTR lpszAddress, int& iAddressLen, USHORT& usPort);
	
private:
		BOOL _send(const DWORD& nID, const BYTE* pBuf, const int& nLen, BOOL bPostQueued = FALSE);
protected:
	typedef std::vector<HANDLE> VECWORKTHREAD;
	typedef std::list<PSOCKET_CONTEXT> LSSOCKETCONTEXT;
	typedef std::list<PIO_CONTEXT> LSIOCONTEXT;
	typedef std::map<DWORD, PSOCKET_CONTEXT> MPSOCKETCONTEXT;
	VECWORKTHREAD		m_vecWorkThread;
	LSSOCKETCONTEXT		m_lsFreeSocketContext;
	LSIOCONTEXT				m_lsFreeIOContext;
	MPSOCKETCONTEXT	m_mpClientSocket;

	CRITICAL_SECTION m_csFreeCxt;
	CRITICAL_SECTION m_csFreeOl;

	CRITICAL_SECTION m_csMapSocket;

	SOCKET m_sockListen;
	HANDLE m_hCompletionPort;
	DWORD m_dwWorkerCount;
	DWORD m_dwAcceptSocketCount;
	DWORD m_dwMaxConnections;
	DWORD m_dwFreeSocketPool;
	DWORD m_dwFreeIOPool;
	DWORD m_dwFreeSocketHold;
	DWORD m_dwFreeIOHold;
	DWORD m_dwMaxShutdownWaitTime;

	volatile BOOL m_bRuning;
	volatile DWORD m_dwConnID;

	LPFN_ACCEPTEX				m_pfnAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS	m_pfnGetAcceptExSockaddrs;
	LPFN_DISCONNECTEX			m_pfnDisconnectEx;

	CSocketRecevier *m_pRecevier;

protected:
	PSOCKET_CONTEXT GetFreeSocketContext(DWORD dwConnID, SOCKET sockClient);
	PIO_CONTEXT GetFreeIOContext(int nLen = 0);
	void AddFreeSocketContext(DWORD dwConnID, ESocketCloseFlag eFlag = SCF_NULL, ESocketOperation eSO = SO_NULL, DWORD dwErrorCode = 0);
	void AddFreeIOContext(PIO_CONTEXT pIO);
	void RemoveSocketContext(PSOCKET_CONTEXT pSock);
	void RemoveIOContext(PIO_CONTEXT pIO);

	void AddClientSocket(DWORD dwConnID, PSOCKET_CONTEXT pSock);
	void RemoveClientSocket(PSOCKET_CONTEXT pSock, ESocketCloseFlag eFlag = SCF_NULL, ESocketOperation eSO = SO_NULL, DWORD dwErrorCode = 0, int nShutdownFlag = SD_SEND);
	PSOCKET_CONTEXT FindSocketContext(DWORD dwConnID);

	int ProcessAccept(SOCKET sockListen, PIO_CONTEXT pIO);
	int ProcessSend(DWORD dwConnID, PSOCKET_CONTEXT pSock, PIO_CONTEXT pIO, DWORD dwIoSize = 0);
	int ProcessReceive(DWORD dwConnID, PSOCKET_CONTEXT pSock, PIO_CONTEXT pIO, DWORD dwIoSize = 0);

	PVOID GetExtensionFuncPtr(SOCKET sock, GUID guid);
	int ManualCloseSocket(SOCKET sock, int nShutDownFlag = 0xFF, BOOL bGraceful = TRUE, BOOL bReuseAddress =FALSE);
	int PostAccept(LPFN_ACCEPTEX pfnAcceptEx, SOCKET sockListen, SOCKET sockClient, PIO_CONTEXT pIO);
	int PostSend(PIO_CONTEXT pIO);
	int PostReceive(PIO_CONTEXT pIO);

	int GetPacketLen(BYTE * pBuf, int nLen);
	int PrepareSendBuf(BYTE * pBufDes, const BYTE * pBufSrc, const int &nLenSrc, DWORD ParamSrc);
	void SaveData(PIO_CONTEXT pIO, const char* pBuf, const int nLen);

private:
	static unsigned int __stdcall WorkerThreadProc(LPVOID lpParam);
};

#endif	// __TCPSERVER_H__