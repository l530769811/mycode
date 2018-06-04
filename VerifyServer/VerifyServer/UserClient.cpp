#include "UserClient.h"


CUserClient::CUserClient(unsigned long socket_id, MyString user_name, MyString user_password)
	: m_socket_id(socket_id)
	, m_user_name(user_name)
	, m_user_password(user_password)
{
}


CUserClient::~CUserClient(void)
{
}

void CUserClient::login_in()
{

}

void CUserClient::login_out()
{

}

bool CUserClient::do_operate(COperater &op)
{
	bool bret = false;


	return bret;
}
