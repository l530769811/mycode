#ifndef _NETSOCKETDATA_H__
#define _NETSOCKETDATA_H__

class CEncrypter;
class CNetSocketProxy;
class CNetSocketData
{
public:
	CNetSocketData(void);
	virtual ~CNetSocketData(void);
	
	int BeSend(CNetSocketProxy *pProxy, CEncrypter *pEncrypter = 0);
protected:
	virtual int _prepare_data(unsigned char* out_data, long len) = 0;
};

#endif //_NETSOCKETDATA_H__

