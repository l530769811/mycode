#include <my_assist_define.h>
#include "ClientManager.h"
#include <DBSqlManager.h>
#include "GlobalFunc.h"
#include "UdpNetSocketProxy.h"
#include "UDPSocket.h"
#include <iterator>
#include <process.h>

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
#include "UserClient.h"
#include "LocalCommunication.h"
#include "NotifyTextCommunicateData.h"
#include "CommunicateData.h"

CClientManager::CClientManager(CDBSqlManager *pdb, CVIPCardManager *pvip_mgr)
	: m_pUdpVerify(NULL)
	, m_pvip_mgr(pvip_mgr)
	, m_db(pdb)
	, m_hcommunication_thread(INVALID_HANDLE_VALUE)
	, m_bthreadRunning(false)
	, m_hEvent(INVALID_HANDLE_VALUE)
{
	m_pUdpVerify = new CUDPSocket(this);
	m_pUdpVerify->Start(UDP_REV_PORT);

	::InitializeCriticalSection(&m_client_lock);
	m_net_parse_list.push_back(new CCheckInLoginInfoNetSocketDataParse(this));
	m_net_parse_list.push_back(new CCheckInLogoutInfoNetSocketDataParse(this));
	m_net_parse_list.push_back(new CVerifyCardNetSocketDataParse(m_pvip_mgr, this));
	m_net_parse_list.push_back(new CRegistCardNetSocketDataParse(m_pvip_mgr, this));
	m_net_parse_list.push_back(new CConsumeCardNetSocketDataParse(m_pvip_mgr, this));

	m_pcommunicater = new CLocalCommunication(_T("verifyserver"), 1);
	
	m_data_vector.push_back(new CNotifyTextCommunicateData);
	::InitializeCriticalSection(&m_criQueueLock);
	m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if(m_hcommunication_thread==INVALID_HANDLE_VALUE || m_hcommunication_thread==0)
	{
		m_bthreadRunning = true;
		m_hcommunication_thread = (HANDLE)_beginthreadex(NULL, 0, _CommunicationThreadProc, reinterpret_cast<void *>(this), 0, 0);
		if(m_hcommunication_thread==INVALID_HANDLE_VALUE || m_hcommunication_thread==0)
		{
			m_bthreadRunning = false;
		}
	}
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

	if (m_hcommunication_thread!=NULL && m_hcommunication_thread!=INVALID_HANDLE_VALUE && m_bthreadRunning==true)
	{
		if(m_pcommunicater!= 0){
			m_pcommunicater->DisconnectedNamePipe();
		}

		m_bthreadRunning = false;
		::WaitForSingleObject(m_hcommunication_thread, 5000);	
		::CloseHandle(m_hcommunication_thread);
		m_hcommunication_thread = INVALID_HANDLE_VALUE;
	}

	::DeleteCriticalSection(&m_criQueueLock);
	if (m_hEvent!=NULL && m_hEvent!=INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hEvent);
	}

	SAFE_DELETE(m_pcommunicater);
	for(std::vector<CCommunicateData*>::iterator it_begin=m_data_vector.begin(); it_begin!=m_data_vector.end(); it_begin++)
	{
		CCommunicateData *pdata = *it_begin;
		delete pdata;
	}
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
		nresult = this->ClientVerify(verify_data) ? 0 : LOGIN_VERIFY_ERROR;
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
			CNotifyTextCommunicateData data(strText);
			this->CommunicateLog(data);
			
		}
	}
	
	return bret;
}


bool CClientManager::ClientVerify(CClientVerifyData &data)
{
	bool ret = false;

	MyString sql = data.ToSql();
	if(m_db!=0)
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
		CNotifyTextCommunicateData data(strText);
		this->CommunicateLog(data);
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
		CNotifyTextCommunicateData data(strText);
		this->CommunicateLog(data);

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
		CNotifyTextCommunicateData data(strText);
		this->CommunicateLog(data);
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

void CClientManager::CommunicateData(CCommunicateData &data)
{
	CLocalCommunication *p = 0;
	try{
		p=m_map_allow_communicate.at(&data);
	}
	catch(std::out_of_range e)
	{
		p = 0;	
	}

	if(p==0)
	{
		m_map_allow_communicate.insert(std::map<CCommunicateData*, CLocalCommunication*>::value_type(&data, m_pcommunicater));
	}
}

void CClientManager::CommunicateLog(CCommunicateData &data)
{
	/*bool bAllow = false;
	CLocalCommunication *pcommunicater = 0;
	std::map<CCommunicateData*, CLocalCommunication*>::iterator it_begin =  m_map_allow_communicate.begin();
	for (; it_begin != m_map_allow_communicate.end(); it_begin++)
	{
		CCommunicateData *pdate = it_begin->first;
		if (pdate!=0)
		{
			unsigned char data_data[1024] = {0};
			int len = 1023;
			data.DecomposeData(data_data, len);
			if(pdate->IsType(data_data, len)==true)
			{
				bAllow = true;
				pcommunicater = it_begin->second;
				break;
			}
		}
	}

	if (bAllow==true && pcommunicater !=0)
	{
		pcommunicater->WriteCommunicationData(0, data);
	}*/
	unsigned char writedata[1024] = {0};
	int nlen = 1023;
	nlen = data.DecomposeData(writedata, nlen);
	PostWrite(writedata, nlen);

}

bool CClientManager::PostWrite(unsigned char *pdata, int nlen)
{
	bool ret = false;
	if(m_bthreadRunning == true && nlen <=write_data::NLEN && m_write_queue.size() < 50){
		::EnterCriticalSection(&m_criQueueLock);
		write_data *pwritedata = new write_data;
		::memcpy(pwritedata->data, pdata, nlen);
		pwritedata->nlen = nlen;
		m_write_queue.push(pwritedata);
		::LeaveCriticalSection(&m_criQueueLock);
		::SetEvent(m_hEvent);
	}
	
	return ret;
}

unsigned int CClientManager::_CommunicationThreadProc(void * pParam)
{
	int ret = -1;
	CClientManager *pApp = reinterpret_cast<CClientManager*>(pParam);
	if(pApp->m_pcommunicater->ConnectedNamedPipe())
	{
		while (pApp->m_bthreadRunning == true)
		{		
			if (pApp->m_write_queue.empty()==true)
			{
				::ResetEvent(pApp->m_hEvent);
				::WaitForSingleObject(pApp->m_hEvent, INFINITE);
			}

			write_data *p = 0;
			::EnterCriticalSection(&pApp->m_criQueueLock);
			p = pApp->m_write_queue.back();
			pApp->m_write_queue.pop();
			::LeaveCriticalSection(&pApp->m_criQueueLock);
			CNotifyTextCommunicateData notify_data;
			notify_data.ComposeData(p->data, p->nlen);
			delete p;
			p = 0;
			pApp->m_pcommunicater->WriteCommunicationData(0, notify_data);

			::Sleep(10);
			ret = 1;
		}
	}

	return ret;
}