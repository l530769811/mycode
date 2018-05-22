#ifndef _SEARCHSERVERNETSOCKETDATA_H__
#define _SEARCHSERVERNETSOCKETDATA_H__

#include "netsocketdata.h"
#include "my_assist_define.h"

class CSearchServerNetSocketData :
	public CNetSocketData
{
public:
	CSearchServerNetSocketData(int port, int kind);
	virtual ~CSearchServerNetSocketData(void);
protected:
	virtual int _prepare_data(unsigned char* out_data, long len);

private:
	MyString m_strServerName;
	MyString m_strIp;
	int m_nport;
	int m_nkind;
};

#endif _SEARCHSERVERNETSOCKETDATA_H__