#ifndef _REGISTCARDNETSOCKETDATAPARSE_H__
#define _REGISTCARDNETSOCKETDATAPARSE_H__

#include "netsocketdataparse.h"
#include "my_assist_define.h"
#include "UseCount.h"

class CVIPCardManager;
class CClientManager;
class CRegistCardNetSocketDataParse :
	public CNetSocketDataParse
{
public:
	CRegistCardNetSocketDataParse(CVIPCardManager *pmgr, CClientManager *pclient_mgr);
	virtual ~CRegistCardNetSocketDataParse(void);

protected:
	virtual bool _isType(const unsigned char* data, long len);
	virtual bool _parseData(unsigned long socket_id, const unsigned char* data, long len);
	CUseCount<COperater> CreateOperater();

private:
	MyString m_strType;
	MyString m_strCardNumber;
	MyString m_strCardPassword;
	MyString m_strCardUserName;
	MyString m_strCardUserPhone;
	int m_nCardUserSex;
	MyString m_strCardUserFirstName;

	CVIPCardManager *m_pmgr;
	CClientManager *m_pclient_mgr;
	unsigned long m_socketid;
	static const MyString m_strRequest;
};

#endif //_REGISTCARDNETSOCKETDATAPARSE_H__