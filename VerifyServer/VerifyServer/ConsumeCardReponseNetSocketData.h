#ifndef _CONSUMECARDREPONSENETSOCKETDATA_H__
#define _CONSUMECARDREPONSENETSOCKETDATA_H__

#include "netsocketdata.h"
#include "my_assist_define.h"

class CConsumeCardReponseNetSocketData :
	public CNetSocketData
{
public:
	CConsumeCardReponseNetSocketData(int result, MyString strCardNumber, MyString strCardPassword);
	virtual ~CConsumeCardReponseNetSocketData(void);

protected:
	virtual int _prepare_data(unsigned char* out_data, long len);

private:
	int m_result;
	MyString m_strCardNumber;
	MyString m_strCardpassword;
};

#endif _CONSUMECARDREPONSENETSOCKETDATA_H__