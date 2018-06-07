#ifndef _VERIFYCARDNETSOCKETDATAPARSSE_H__
#define _VERIFYCARDNETSOCKETDATAPARSSE_H__

#include "netsocketdataparse.h"
#include "my_assist_define.h"
#include "UseCount.h"

class CVIPCardManager;
class CClientManager;
class CVerifyCardNetSocketDataParse :
	public CNetSocketDataParse
{
public:
	CVerifyCardNetSocketDataParse(CVIPCardManager *pmgr, CClientManager *pclient_mgr);
	virtual ~CVerifyCardNetSocketDataParse(void);

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

#endif _VERIFYCARDNETSOCKETDATAPARSSE_H__