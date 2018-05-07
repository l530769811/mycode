#ifndef _CLIENTVERIFYNETSOCKETDATAPARSE_H__
#define _CLIENTVERIFYNETSOCKETDATAPARSE_H__

#include "netsocketdataparse.h"
#include "my_assist_define.h"


class CClientManager;
class CClientVerifyNetSocketDataParse :
	public CNetSocketDataParse
{
public:
	CClientVerifyNetSocketDataParse(CClientManager *clientMgr);
	~CClientVerifyNetSocketDataParse(void);

public:
	virtual void RefreshOperator(COperater *operate);

protected:
	virtual bool _isType(const unsigned char* data, long len);
	virtual bool _parseData(const unsigned char* data, long len);

private:
	MyString m_strType;
	MyString m_strSql;
	
	COperater *m_operator;
	CClientManager *m_clientMgr;

	static const MyString m_strRequest;
};

#endif //_CLIENTVERIFYNETSOCKETDATAPARSE_H__