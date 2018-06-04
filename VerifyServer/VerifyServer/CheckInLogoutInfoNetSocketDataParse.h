#pragma once
#include "netsocketdataparse.h"
#include "my_assist_define.h"
#include "UseCount.h"

class CClientManager;
class CCheckInLogoutInfoNetSocketDataParse :
	public CNetSocketDataParse
{
public:
	CCheckInLogoutInfoNetSocketDataParse(CClientManager *mgr);
	virtual ~CCheckInLogoutInfoNetSocketDataParse(void);

protected:
	virtual bool _isType(const unsigned char* data, long len);
	virtual bool _parseData(unsigned long socket_id, const unsigned char* data, long len);
	CUseCount<COperater> CreateOperater();

private:
	MyString m_strType;
	MyString m_strUserName;
	MyString m_strUserPassword;

	CClientManager *m_pgmr;
	unsigned long m_socketid;
	static const MyString m_strRequest ;
};

