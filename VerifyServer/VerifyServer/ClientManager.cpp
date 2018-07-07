#include <my_assist_define.h>
#include "ClientManager.h"
#include <DBSqlManager.h>
#include "GlobalFunc.h"
#include "UdpNetSocketProxy.h"
#include "UDPSocket.h"

#include "UserClient.h"
#include "LocalCommunication.h"
#include "NotifyTextCommunicateData.h"
#include "CommunicateData.h"
#include "veriry_common_define.h"
#include "SignupMethods.h"
#include "DBSqlExecOperate.h"
#include "NetSocketData.h"
#include "NetSocketProxy.h"
#include "LoginVerifyReponseNetSocketData.h"
#include "UdpNetSocketProxy.h"
#include "ClientVerifyOperator.h"
#include "ClientVerifyNetSocketDataParse.h"
#include "SearchServerNetSocketDataParse.h"
#include "SearchServerNetSocketData.h"
#include "error_id_define.h"
#include "ClientSignupNetSocketDataParse.h"
#include "ClientSignupDBDoOperator.h"
#include "SignupReponseNetSocketData.h"
#include "CheckInLoginInfoNetSocketDataParse.h"
#include "CheckInLogoutInfoNetSocketDataParse.h"
#include "VerifyCardNetSocketDataParse.h"
#include "RegistCardNetSocketDataParse.h"
#include "ConsumeCardNetSocketDataParse.h"
#include "ProcessLogCollecter.h"
#include "CheckoutUserDBDoOperator.h"

CClientManager::CClientManager(CDBSqlManager *pdb, CVIPCardManager *pvip_mgr)
	: m_pUdpVerify(NULL)
	, m_pvip_mgr(pvip_mgr)
	, m_db(pdb)
{
	m_pUdpVerify = new CUDPSocket(this);
	m_pUdpVerify->Start(UDP_REV_PORT);

	::InitializeCriticalSection(&m_client_lock);
	m_net_parse_list.push_back(new CCheckInLoginInfoNetSocketDataParse(this));
	m_net_parse_list.push_back(new CCheckInLogoutInfoNetSocketDataParse(this));
	m_net_parse_list.push_back(new CVerifyCardNetSocketDataParse(m_pvip_mgr, this));
	m_net_parse_list.push_back(new CRegistCardNetSocketDataParse(m_pvip_mgr, this));
	m_net_parse_list.push_back(new CConsumeCardNetSocketDataParse(m_pvip_mgr, this));

	
}

bool CClientManager::Open()
{
	bool bret = false;

	return bret;
}

bool CClientManager::Close()
{
	bool bret = false;

	return bret;
}


CClientManager::~CClientManager(void)
{
	SAFE_DELETE(m_pUdpVerify);
	std::list<CNetSocketDataParse*>::iterator it_begin = m_net_parse_list.begin();
	for(; it_begin!=m_net_parse_list.end(); it_begin++)
	{
		CNetSocketDataParse* p = *it_begin;
		delete p;
		p = 0;
	}

	for(std::map<MyString, CClient*>::iterator it_begin=m_client_list.begin(); it_begin!=m_client_list.end(); it_begin++)
	{
		 CClient* p = it_begin->second;
		 delete p;
	}
	::DeleteCriticalSection(&m_client_lock);
}

int CClientManager::rev_data(const unsigned char * data, long len, char *ip_from, unsigned short port_from){
	int nret = 0;
	
	TCHAR str_ip[20] = {0};
	GlobalUtf8ToUnicode(ip_from, str_ip, 19);

	CClientVerifyNetSocketDataParse verify_parse(this);
	if (verify_parse.ParseData(0, data, len)==true)
	{
		CClientVerifyData verify_data = verify_parse.GetVerifyData();
		int nresult = 0;
		nresult = this->ClientVerify(verify_data);
		CLoginVerifyReponseNetSocketData socket_data(nresult);
		CUdpNetSocketProxy proxy(this->m_pUdpVerify, str_ip, port_from);
		this->ClientReponse(socket_data, proxy);
	}

	CSearchServerNetSocketDataParse search_server_parse;
	if (search_server_parse.ParseData(0, data, len) == true)
	{
		CSearchServerNetSocketData socket_data(UDP_REV_PORT, -1);
		
		CUdpNetSocketProxy proxy(this->m_pUdpVerify, str_ip, port_from);
		this->ClientReponse(socket_data, proxy);
	}
	
	CClientSignupNetSocketDataParse signup_parse(this);
	if (signup_parse.ParseData(0, data, len) == true)
	{
		CClientSignupData signup_data = signup_parse.GetSignupData();
		
		int nresult = -1;
		nresult = this->ClientSignup(signup_data, 0) == true ?  0 : SIGNUP_ERROR;
		CSignupReponseNetSocketData socket_data(nresult);
		CUdpNetSocketProxy proxy(this->m_pUdpVerify, str_ip, port_from);
		this->ClientReponse(socket_data, proxy);
	}

	return nret;
}

void CClientManager::Recevie(unsigned long uSokcetID, BYTE *rev_buf, UINT rev_len)
{
	int i = 0;
	std::list<CNetSocketDataParse*>::iterator it_begin = m_net_parse_list.begin();
	for(; it_begin!=m_net_parse_list.end(); it_begin++)
	{
			CNetSocketDataParse* p = *it_begin;
			if(p!=0&& p->ParseData(uSokcetID, rev_buf, rev_len)==true)
			{
				CUseCount<COperater> op = p->CreateOperater();
				op->Operate();
				break;
			}
	}
}

void CClientManager::connect_coming(unsigned long socketid, unsigned int nport){

}

void CClientManager::unconnect_coming(unsigned long socketid, unsigned int nport){

	DeleteConnectClient(socketid);
}

bool CClientManager::ClientSignup(CClientSignupData &data,  CSignupMethods *signupMethods)
{
	bool bret = false;
	if(signupMethods!=0 &&signupMethods->signupMethodVerify())
	{
		MyString sql = data.ToSql();
		if(m_db!=0)
		{
			CClientSignupDBDoOperator signupReponse(sql);
			bret = signupReponse.Exec(m_db) ==0 ? true : false;			
		}
	}
	else
	{
		MyString sql = data.ToSql();
		if(m_db!=0)
		{
			CClientSignupDBDoOperator signupReponse(sql);
			bret = signupReponse.Exec(m_db) ==0 ? true : false;
			MyString strText;
			if (bret == true)
			{
				
				strText = strText + _T("signup client : ") + data.GetName() + _T("	fail");
			}
			else
			{
				strText = strText + _T("signup client : ") + data.GetName() + _T("	done");
			}
			log(TEXT_LOG, strText);
			
		}
	}
	
	return bret;
}


int CClientManager::ClientVerify(CClientVerifyData &data)
{
	int ret = NO_ERROR;

	MyString sql = data.ToSql();
	if(m_db!=0)
	{	
		CCheckoutClientDBDoOperator check_op(data.GetName());
		if( (ret = check_op.GetCheckoutResult()) ==NO_ERROR)
		{

			CClientVerifyOperator veryfyReponse(sql);
			veryfyReponse.Exec(m_db);
			ret=veryfyReponse.GetVerifyResult();
			MyString strText;
			if (ret == NO_ERROR)
			{

				strText = strText + _T("Verify client : ") + data.GetName() + _T("	fail");

			}
			else
			{
				strText = strText + _T("Verify client : ") + data.GetName() + _T("	done");
			}
			log(TEXT_LOG, strText);
		}
	}

	return ret;
}

bool CClientManager::ClientReponse(CNetSocketData &data, CNetSocketProxy &proxy)
{
	bool ret = false;
	ret = data.BeSend(&proxy) > 0;
	return ret;
}

int CClientManager::AddConnectClient(unsigned long nid, MyString strUserName, MyString strUserPassword)
{
	int ret = -1;
	CClient* pcur = 0;
	::EnterCriticalSection(&m_client_lock);
	try{
		pcur = m_client_list.at(strUserName);
	}
	catch(out_of_range e)
	{
		pcur = 0;
	}

	if ( pcur==0 )
	{
		MyString strText;
		strText = strText + _T("Login client : ") + strUserName;
		log(TEXT_LOG, strText);

		CClient* puser = new CUserClient(nid, strUserName, strUserPassword);
		m_client_list.insert(map<MyString, CClient*>::value_type(strUserName, puser));
		m_client_id_list.insert(map<unsigned long, CClient*>::value_type(nid, puser));
		ret = 0;
	}
	::LeaveCriticalSection(&m_client_lock);
	return ret;
}

int CClientManager::DeleteConnectClient(unsigned long nid, MyString strUserName, MyString strUserPassword)
{
	int ret = -1;
	CClient* pcur = 0;
	::EnterCriticalSection(&m_client_lock);
	try{
		pcur = m_client_list.at(strUserName);
	}
	catch(out_of_range e)
	{
		pcur = 0;
	}

	if ( pcur!=0 )
	{
		MyString strText;
		strText = strText + _T("Logout client : ") + strUserName;
		log(TEXT_LOG, strText);

		m_client_id_list.erase(nid);
		m_client_list.erase(strUserName);
		delete pcur;
		ret = 0;
	}
	::LeaveCriticalSection(&m_client_lock);

	return ret;
}

int CClientManager::DeleteConnectClient(unsigned long nid)
{
	int ret = -1;
	CClient* p = 0;
	::EnterCriticalSection(&m_client_lock);
	try{
		p = m_client_id_list.at(nid);
	}
	catch(out_of_range e)
	{
		p = 0;
	}
	
	std::map<MyString, CClient*>::iterator it_begin =  m_client_list.begin();
	for (; it_begin != m_client_list.end(); it_begin++)
	{
		map<MyString, CClient*>::value_type pvalue = *it_begin;
		if (pvalue.second == p)
		{
			m_client_id_list.erase(nid);
			m_client_list.erase(it_begin);
			delete p;
			ret = 0;
			break;
		}
	}
	::LeaveCriticalSection(&m_client_lock);
	return ret;
}

CClient* CClientManager::FindConnectClient(unsigned long nid)
{
	CClient* pclient = 0;
	::EnterCriticalSection(&m_client_lock);
	CClient* p = m_client_id_list.at(nid);
	if(p!=0){
		std::map<MyString, CClient*>::iterator it_begin =  m_client_list.begin();
		for (; it_begin != m_client_list.end(); it_begin++)
		{
			map<MyString, CClient*>::value_type pvalue = *it_begin;
			if (pvalue.second == p)
			{
				//m_client_id_list.erase(nid);
				//m_client_list.erase(it_begin);
				//delete p;
				pclient = p;
				break;
			}
		}
	}

	::LeaveCriticalSection(&m_client_lock);
	return pclient;
}