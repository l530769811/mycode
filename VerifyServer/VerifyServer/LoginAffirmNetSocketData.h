#ifndef _LOGINAFFIRMNETSOCKETDATA_H__
#define _LOGINAFFIRMNETSOCKETDATA_H__

#include "netsocketdata.h"
#include "my_assist_define.h"

class CLoginAffirmNetSocketData :
	public CNetSocketData
{
public:
	CLoginAffirmNetSocketData(int result, MyString user_name, MyString user_password);
	virtual ~CLoginAffirmNetSocketData(void);
protected:
	int _prepare_data(unsigned char* out_data, long len);

private:
	MyString m_strUserName;
	MyString m_strUserPassword;

	int m_result;

};

#endif _LOGINAFFIRMNETSOCKETDATA_H__