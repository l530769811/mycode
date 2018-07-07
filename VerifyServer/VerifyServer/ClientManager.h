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
	int ClientVerify(CClientVerifyData &data);
	bool ClientReponse(CNetSocketData &data, CNetSocketProxy &proxy);

	int AddConnectClient(unsigned long nid, MyString strUserName, MyString strUserPassword);
	int DeleteConnectClient(unsigned long nid, MyString strUserName, MyString strUserPassword);
	int DeleteConnectClient(unsigned long nid);
	CClient* FindConnectClient(unsigned long nid);

	bool Open();
	bool Close();

private:
	CUDPSocket * m_pUdpVerify;
	CVIPCardManager *m_pvip_mgr;
	CDBSqlManager *m_db;
	std::list<CNetSocketDataParse*> m_net_parse_list; 

	CRITICAL_SECTION m_client_lock;
	std::map<MyString, CClient*> m_client_list;
	std::map<unsigned long, CClient*> m_client_id_list;


};

#endif //__CLIENT_MANAGER_H__