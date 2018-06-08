#include "RegistCardNetSocketDataParse.h"
#include "cJSON_Unicode.h"
#include "SocketProtocol.h"
#include "GlobalFunc.h"
#include "RegistCardOperater.h"

const MyString CRegistCardNetSocketDataParse::m_strRequest = _T("card_regist_request");
CRegistCardNetSocketDataParse::CRegistCardNetSocketDataParse(CVIPCardManager *pmgr, CClientManager *pclient_mgr)
	: m_pmgr(pmgr)
	, m_pclient_mgr(pclient_mgr)
{
}


CRegistCardNetSocketDataParse::~CRegistCardNetSocketDataParse(void)
{
}


bool CRegistCardNetSocketDataParse::_isType(const unsigned char* data, long len){
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

bool CRegistCardNetSocketDataParse::_parseData(unsigned long socket_id, const unsigned char* data, long len){
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
				
				cJSON *p = cJSON_GetObjectItem(pvalue, JSON_CARD_NUMBER_KEY);
				if (p!=0 && p->type==cJSON_String)
				{
					m_strCardNumber = p->valuestring;
				}

				p = cJSON_GetObjectItem(pvalue, JSON_CARD_PASSWORD_KEY);
				if (p!=0 && p->type==cJSON_String)
				{
					m_strCardPassword = p->valuestring;
				}

				p = cJSON_GetObjectItem(pvalue, JSON_CARD_PASSWORD_ENCODE_KEY);
				if (p!=0 && p->type==cJSON_String)
				{
					m_strCardPasswordEncode = p->valuestring;
				}

				p = cJSON_GetObjectItem(pvalue, JSON_CARD_USER_NAME_KEY);
				if (p!=0 && p->type==cJSON_String)
				{
					m_strCardUserName = p->valuestring;
				}

				p = cJSON_GetObjectItem(pvalue, JSON_PHONE_NUMBER_KEY);
				if (p!=0 && p->type==cJSON_String)
				{
					m_strCardUserPhone = p->valuestring;
				}

				p = cJSON_GetObjectItem(pvalue, JSON_SEX_KEY);
				if (p!=0 && p->type==cJSON_Number)
				{
					m_nCardUserSex = p->valueint;
				}

				p = cJSON_GetObjectItem(pvalue, JSON_FIRST_NAME_KEY);
				if (p!=0 && p->type==cJSON_String)
				{
					m_strCardUserFirstName = p->valuestring;
				}
			}
		}
	}

	return bret;
}

CUseCount<COperater> CRegistCardNetSocketDataParse::CreateOperater()
{
	return CUseCount<COperater>( new CRegistCardOperater(m_pmgr, m_pclient_mgr, m_socketid,  m_strCardNumber, m_strCardPassword, m_strCardPasswordEncode, m_strCardUserName, m_strCardUserPhone, m_strCardUserFirstName, m_nCardUserSex));
}