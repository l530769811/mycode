#include "SearchServerNetSocketDataParse.h"
#include "GlobalFunc.h"
#include "cJSON_Unicode.h"
#include "SocketProtocol.h"
#include "sqlite_sql.h"

const MyString CSearchServerNetSocketDataParse::m_strRequest = _T("search_server_request");
CSearchServerNetSocketDataParse::CSearchServerNetSocketDataParse(void)
	: m_nPort(0)
{
}


CSearchServerNetSocketDataParse::~CSearchServerNetSocketDataParse(void)
{
}

bool CSearchServerNetSocketDataParse::_isType(const unsigned char* data, long len){
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

bool CSearchServerNetSocketDataParse::_parseData(const unsigned char* data, long len){
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
				cJSON* p = cJSON_GetObjectItem(pvalue, JSON_IP_KEY);
				if (p!=0 && p->type==cJSON_String)
				{
					m_strIp = p->valuestring;
				}
				
				p = cJSON_GetObjectItem(pvalue, JSON_PORT_KEY);
				if (p!=0 && p->type==cJSON_Number)
				{
					m_nPort = p->valueint;
				}
			}
		}
	}

	return bret;
}

void CSearchServerNetSocketDataParse::RefreshOperator(COperater *operate){
	if(operate!=0){
		//CDBSqlExecOperate o;
		//operate->Copy(o);
	}
}