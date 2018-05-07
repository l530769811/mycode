#ifndef __CLIENT_MANAGER_H__
#define __CLIENT_MANAGER_H__
#include <UdpSocketReceiveInterface.h>
#include <SocketRecevier.h>
#include "SignupData.h"
#include "ClientVerifyData.h"
#include <UseCount.h>
#include <UseCount.cpp>

class CUDPSocket;
class CSignupMethods;
class COperater;
class CDBSqlManager;
class CClientVerifyData;
class CNetSocketData;
class CNetSocketProxy;
class CClientVerifyReponseOperater;
class CClientManager : 
	public CUdpSocketReceiveInterface,
	public CSocketRecevier
{
public:
	CClientManager(CDBSqlManager *pdb);
	virtual ~CClientManager(void);
	virtual int rev_data(const unsigned char * data, long len, char *ip_form, unsigned short port_from);
	virtual void Recevie(DWORD uSokcetID, BYTE *rev_buf, UINT rev_len);
	virtual void connect_coming(DWORD socketid, unsigned int nport);
	virtual void unconnect_coming(DWORD socketid, unsigned int nport);

public:
	bool ClientSignup(CUseCount<CClientSignupData> data,  CUseCount< CSignupMethods> signupMethods);
	bool ClientVerify(CUseCount<CClientVerifyData> data);
	bool ClientReponse(CNetSocketData &data, CNetSocketProxy &proxy);

	bool Open();
	bool Close();

//protected:
//	static int ClientVerifyCallback(void *data, int argc, char **argv, char **azColName);

private:
	CUDPSocket * m_pUdpVerify;
	CDBSqlManager *m_db;
	
	CClientVerifyReponseOperater * pveryfyReponse;
};

#endif //__CLIENT_MANAGER_H__