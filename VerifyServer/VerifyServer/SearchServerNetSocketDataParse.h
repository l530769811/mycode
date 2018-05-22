#ifndef _SEARCHSERVERNETSOCKETDATAPARSE_H__
#define _SEARCHSERVERNETSOCKETDATAPARSE_H__

#include "netsocketdataparse.h"
#include <my_assist_define.h>
class CSearchServerNetSocketDataParse :
	public CNetSocketDataParse
{
public:
	CSearchServerNetSocketDataParse(void);
	virtual ~CSearchServerNetSocketDataParse(void);
public:
	virtual void RefreshOperator(COperater *operate);

protected:
	virtual bool _isType(const unsigned char* data, long len);
	virtual bool _parseData(const unsigned char* data, long len);

private:
	MyString m_strType;
	MyString m_strIp;
	int m_nPort;

	static const MyString m_strRequest;
};

#endif //_SEARCHSERVERNETSOCKETDATAPARSE_H__