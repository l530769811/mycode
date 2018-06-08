#ifndef _CONSUMECARDNETSOCKETDATAPARSE_H__
#define _CONSUMECARDNETSOCKETDATAPARSE_H__

#include "netsocketdataparse.h"
#include "my_assist_define.h"
#include "UseCount.h"

class CVIPCardManager;
class CClientManager;
class CConsumeCardNetSocketDataParse :
	public CNetSocketDataParse
{
public:
	CConsumeCardNetSocketDataParse(CVIPCardManager *pmgr, CClientManager *pclient_mgr);
	virtual ~CConsumeCardNetSocketDataParse(void);

protected:
	virtual bool _isType(const unsigned char* data, long len);
	virtual bool _parseData(unsigned long socket_id, const unsigned char* data, long len);
	CUseCount<COperater> CreateOperater();

private:
	MyString m_strType;
	MyString m_strCardNumber;
	MyString m_strCardPassword;

	CVIPCardManager *m_pmgr;
	CClientManager *m_pclient_mgr;
	unsigned long m_socketid;
	static const MyString m_strRequest;
};

#endif _CONSUMECARDNETSOCKETDATAPARSE_H__