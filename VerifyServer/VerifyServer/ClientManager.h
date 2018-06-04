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
class CClientManager : 
	public CUdpSocketReceiveInterface,
	public CSocketRecevier
{
public:
	CClientManager(CDBSqlManager *pdb);
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

	bool Open();
	bool Close();

//protected:
//	static int ClientVerifyCallback(void *data, int argc, char **argv, char **azColName);

private:
	CUDPSocket * m_pUdpVerify;
	CDBSqlManager *m_db;
	std::list<CNetSocketDataParse*> m_net_parse_list; 

	CRITICAL_SECTION m_client_lock;
	std::map<MyString, CClient*> m_client_list;
};

#endif //__CLIENT_MANAGER_H__