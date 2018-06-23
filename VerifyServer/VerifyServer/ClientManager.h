#ifndef __CLIENT_MANAGER_H__
#define __CLIENT_MANAGER_H__
#include <UdpSocketReceiveInterface.h>
#include <SocketRecevier.h>
#include "SignupData.h"
#include "ClientVerifyData.h"
#include <list>
#include <map>
#include <UseCount.h>
#include <UseCount.cpp>
#include <vector>
#include <map>
#include <queue>
#include "MemAlloctor.h"

class CUDPSocket;
class CSignupMethods;
class COperater;
class CDBSqlManager;
class CClientVerifyData;
class CNetSocketData;
class CNetSocketProxy;
class CClientVerifyOperator;
class CNetSocketDataParse;
class CClient;
class CVIPCardManager;
class CLocalCommunication;
class CCommunicateData;
struct write_data;
class CClientManager : 
	public CUdpSocketReceiveInterface,
	public CSocketRecevier
{
public:
	CClientManager(CDBSqlManager *pdb, CVIPCardManager *pvip_mgr);
	virtual ~CClientManager(void);
	virtual int rev_data(const unsigned char * data, long len, char *ip_form, unsigned short port_from);
	virtual void Recevie(unsigned long uSokcetID, BYTE *rev_buf, UINT rev_len);
	virtual void connect_coming(unsigned long socketid, unsigned int nport);
	virtual void unconnect_coming(unsigned long socketid, unsigned int nport);

public:
	bool ClientSignup(CClientSignupData &data,  CSignupMethods *signupMethods);
	bool ClientVerify(CClientVerifyData &data);
	bool ClientReponse(CNetSocketData &data, CNetSocketProxy &proxy);

	int AddConnectClient(unsigned long nid, MyString strUserName, MyString strUserPassword);
	int DeleteConnectClient(unsigned long nid, MyString strUserName, MyString strUserPassword);
	int DeleteConnectClient(unsigned long nid);
	CClient* FindConnectClient(unsigned long nid);

	bool Open();
	bool Close();

//protected:
//	static int ClientVerifyCallback(void *data, int argc, char **argv, char **azColName);
public:
	void CommunicateData(CCommunicateData &data);
	void CommunicateLog(CCommunicateData &data);

private:
	static unsigned int __stdcall _CommunicationThreadProc(void * pParam);
	bool PostWrite(unsigned char *pdata, int nlen);


private:
	CUDPSocket * m_pUdpVerify;
	CVIPCardManager *m_pvip_mgr;
	CDBSqlManager *m_db;
	std::list<CNetSocketDataParse*> m_net_parse_list; 

	CRITICAL_SECTION m_client_lock;
	std::map<MyString, CClient*> m_client_list;
	std::map<unsigned long, CClient*> m_client_id_list;

	CLocalCommunication *m_pcommunicater;
	HANDLE m_hcommunication_thread;
	bool m_bthreadRunning;
	std::vector<CCommunicateData*> m_data_vector;
	std::map<CCommunicateData*, CLocalCommunication*> m_map_allow_communicate;

	struct write_data :public CMemAlloctor{
		static const int NLEN = 1024;
		unsigned char data[1024];
		int nlen;
	};
	std::queue<write_data*> m_write_queue;
	CRITICAL_SECTION m_criQueueLock;
	HANDLE m_hEvent;
};

#endif //__CLIENT_MANAGER_H__