#ifndef _NETSOCKETDATAPARSE_H__
#define _NETSOCKETDATAPARSE_H__

class CEncrypter;
class COperater;
class CNetSocketDataParse
{
public:
	CNetSocketDataParse(void);
	virtual ~CNetSocketDataParse(void);

	bool ParseData(const unsigned char* data, long len, CEncrypter *pEncrypter = 0);
	virtual void RefreshOperator(COperater *operate);

protected:
	virtual bool _isType(const unsigned char* data, long len) = 0;
	virtual bool _parseData(const unsigned char* data, long len) = 0;
	//virtual COperater CreateOperater() = 0;
};

#endif //_NETSOCKETDATAPARSE_H__