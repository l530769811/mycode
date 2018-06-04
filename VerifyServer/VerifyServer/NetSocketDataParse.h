#ifndef _NETSOCKETDATAPARSE_H__
#define _NETSOCKETDATAPARSE_H__
#include "UseCount.h"

class CEncrypter;
class COperater;
class CNetSocketDataParse
{
public:
	CNetSocketDataParse(void);
	virtual ~CNetSocketDataParse(void);

	bool ParseData(unsigned long socket_id, const unsigned char* data, long len, CEncrypter *pEncrypter = 0);
	virtual CUseCount<COperater> CreateOperater() = 0;

protected:
	virtual bool _isType(const unsigned char* data, long len) = 0;
	virtual bool _parseData(unsigned long socket_id, const unsigned char* data, long len) = 0;
	
};

#endif //_NETSOCKETDATAPARSE_H__