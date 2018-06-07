#ifndef _VERIFYCARDREPONSENETSOCKETDATA_H__
#define _VERIFYCARDREPONSENETSOCKETDATA_H__
#include "netsocketdata.h"
#include "my_assist_define.h"

class CVerifyCardReponseNetSocketData :
	public CNetSocketData
{
public:
	CVerifyCardReponseNetSocketData(int result, MyString strResult, MyString strCardNumber, MyString strCardPassword);
	virtual ~CVerifyCardReponseNetSocketData(void);

protected:
	virtual int _prepare_data(unsigned char* out_data, long len);

private:
	int m_result;
	MyString m_strCardNumber;
	MyString m_strCardpassword;
	MyString m_strResult;
};

#endif _VERIFYCARDREPONSENETSOCKETDATA_H__