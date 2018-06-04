#ifndef _CLIENTVERIFYNETSOCKETDATAPARSE_H__
#define _CLIENTVERIFYNETSOCKETDATAPARSE_H__

#include "netsocketdataparse.h"
#include "my_assist_define.h"
#include "UseCount.h"

class CClientVerifyData;
class CClientManager;
class CClientVerifyNetSocketDataParse :
	public CNetSocketDataParse
{
public:
	CClientVerifyNetSocketDataParse(CClientManager *clientMgr);
	~CClientVerifyNetSocketDataParse(void);
	CClientVerifyData GetVerifyData();

protected:
	virtual bool _isType(const unsigned char* data, long len);
	virtual bool _parseData(unsigned long socket_id, const unsigned char* data, long len);
	virtual CUseCount<COperater> CreateOperater();

private:
	MyString m_strType;
	MyString m_strSql;
	MyString m_strUserName;
	MyString m_strUserPassword;
	
	COperater *m_operator;
	CClientManager *m_clientMgr;

	static const MyString m_strRequest;
};

#endif //_CLIENTVERIFYNETSOCKETDATAPARSE_H__