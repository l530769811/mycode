#include "ClientVerifyNetSocketDataParse.h"
#include "GlobalFunc.h"
#include "cJSON_Unicode.h"
#include "SocketProtocol.h"
#include "sqlite_sql.h"
#include "ClientManager.h"
#include "ClientVerifyData.h"
#include "UseCount.h"


const MyString CClientVerifyNetSocketDataParse::m_strRequest = _T("verify_login_request");
CClientVerifyNetSocketDataParse::CClientVerifyNetSocketDataParse(CClientManager *clientMgr)
	: m_clientMgr(clientMgr)
{
}


CClientVerifyNetSocketDataParse::~CClientVerifyNetSocketDataParse(void)
{
}



bool CClientVerifyNetSocketDataParse::_isType(const unsigned char* data, long len){
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

bool CClientVerifyNetSocketDataParse::_parseData(unsigned long socket_id, const unsigned char* data, long len){
	bool bret = false;

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

				TCHAR sql[MAX_PATH] = {0};
				_stprintf(sql, MAX_PATH-1, select_Login, strUserName.c_str(), strUserPassword.c_str());
				this->m_strSql = sql;
				m_strUserName = strUserName;
				m_strUserPassword = strUserPassword;

			}
		}
	}

	return bret;
}

CUseCount<COperater>  CClientVerifyNetSocketDataParse::CreateOperater()
{
	return CUseCount<COperater>(0);
}

CClientVerifyData CClientVerifyNetSocketDataParse::GetVerifyData()
{
	return CClientVerifyData(m_strUserName, m_strUserPassword);
}
