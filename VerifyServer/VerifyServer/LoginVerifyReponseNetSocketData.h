#ifndef _LOGINVERIFYREPONSENETSOCKETDATA_H__
#define _LOGINVERIFYREPONSENETSOCKETDATA_H__
#include "NetSocketData.h"

class CLoginVerifyReponseNetSocketData : public  CNetSocketData
{
public:
	CLoginVerifyReponseNetSocketData(int result);
	virtual ~CLoginVerifyReponseNetSocketData(void);

protected:
	virtual int _prepare_data(unsigned char* out_data, long len);

private:
	int m_result;
};

#endif //_LOGINVERIFYREPONSENETSOCKETDATA_H__