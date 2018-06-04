#include "CheckInLogoutInfoNetSocketDataParse.h"
#include "cJSON_Unicode.h"
#include "SocketProtocol.h"
#include "GlobalFunc.h"
#include "CheckInLogoutInfoOperater.h"


const MyString CCheckInLogoutInfoNetSocketDataParse::m_strRequest = _T("checkin_login_out");
CCheckInLogoutInfoNetSocketDataParse::CCheckInLogoutInfoNetSocketDataParse(CClientManager *mgr)
	: m_pgmr(mgr)
	, m_socketid(-1)
{
}


CCheckInLogoutInfoNetSocketDataParse::~CCheckInLogoutInfoNetSocketDataParse(void)
{
}


bool CCheckInLogoutInfoNetSocketDataParse::_isType(const unsigned char* data, long len){
	bool bret = false;
	TCHAR unicode[1024] = {0};
	GlobalUtf8ToUnicode((const char*)data, unicode, 1023);
	cJSON *proot = cJSON_Parse(unicode);
	if(proot!=0){
		cJSON *pcontent = cJSON_GetObjectItem(proot, JSON_COMMON_CONTENT_KEY);
		if (pcontent!=0)
		{
			cJSON *ptype = cJSON_GetObjectItem(pcontent, JSON_COMMON_CTYPE_KEY);
			if(ptype!=0 && ptype->type == cJSON_String){
				m_strType = ptype->valuestring;
				bret = m_strType.compare(m_strRequest)==0;
			}
		}
	}
	return bret;
}

bool CCheckInLogoutInfoNetSocketDataParse::_parseData(unsigned long socket_id, const unsigned char* data, long len){
	bool bret = false;
	m_socketid = socket_id;
	TCHAR unicode[1024] = {0};
	GlobalUtf8ToUnicode((const char*)data, unicode, 1023);
	cJSON *proot = cJSON_Parse(unicode);
	if(proot!=0){
		cJSON *pcontent = cJSON_GetObjectItem(proot, JSON_COMMON_CONTENT_KEY);
		if (pcontent!=0)
		{
			cJSON *pvalue = cJSON_GetObjectItem(pcontent, JSON_COMMON_CVALUE_KEY);
			if(pvalue!=0 && pvalue->type == cJSON_Object){
				MyString strUserName = _T("");
				MyString strUserPassword = _T("");
				MyString strUserPhone = _T("");
				MyString strUserIdentifyCode = _T("");

				cJSON *p = cJSON_GetObjectItem(pvalue, JSON_USER_NAME_KEY);
				if (p!=0 && p->type==cJSON_String)
				{
					strUserName = p->valuestring;
				}

				p = cJSON_GetObjectItem(pvalue, JSON_USER_PASSWORD_KEY);
				if (p!=0 && p->type==cJSON_String)
				{
					strUserPassword = p->valuestring;
				}

				m_strUserName = strUserName;
				m_strUserPassword = strUserPassword;
			}
		}
	}

	return bret;
}

CUseCount<COperater> CCheckInLogoutInfoNetSocketDataParse::CreateOperater()
{
	return CUseCount<COperater>( new CCheckInLogoutInfoOperater(m_pgmr, m_socketid, m_strUserName, m_strUserPassword));
}