#pragma once
#include "client.h"
#include "my_assist_define.h"

class COperater;
class CUserClient :
	public CClient
{
public:
	CUserClient(unsigned long socket_id, MyString user_name, MyString user_password);
	virtual ~CUserClient(void);

	virtual void login_in();
	virtual void login_out();
	virtual bool do_operate(COperater &op);

private:
	unsigned long m_socket_id;
	MyString m_user_name;
	MyString m_user_password;
};

