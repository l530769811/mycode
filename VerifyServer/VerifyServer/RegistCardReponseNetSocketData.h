#ifndef _REGISTCARDREPONSENETSOCKETDATA_H__
#define _REGISTCARDREPONSENETSOCKETDATA_H__

#include "netsocketdata.h"
#include "my_assist_define.h"
class CRegistCardReponseNetSocketData :
	public CNetSocketData
{
public:
	CRegistCardReponseNetSocketData(int result, MyString strCardNumber, MyString strCardPassword);
	virtual ~CRegistCardReponseNetSocketData(void);

protected:
	virtual int _prepare_data(unsigned char* out_data, long len);

private:
	int m_result;
	MyString m_strCardNumber;
	MyString m_strCardpassword;
};

#endif _REGISTCARDREPONSENETSOCKETDATA_H__