#ifndef _SIGNUPREPONSENETSOCKETDATA_H__
#define _SIGNUPREPONSENETSOCKETDATA_H__

#include "netsocketdata.h"
class CSignupReponseNetSocketData :
	public CNetSocketData
{
public:
	CSignupReponseNetSocketData(int result);
	virtual ~CSignupReponseNetSocketData(void);

protected:
	virtual int _prepare_data(unsigned char* out_data, long len);

private:
	int m_result;
};

#endif //_SIGNUPREPONSENETSOCKETDATA_H__