#include "SearchServerNetSocketData.h"
#include "cJSON_Unicode.h"
#include "SocketProtocol.h"
#include "GlobalFunc.h"

CSearchServerNetSocketData::CSearchServerNetSocketData(int port, int kind)
	: m_nport(port) 
	, m_nkind(kind)
{
}


CSearchServerNetSocketData::~CSearchServerNetSocketData(void)
{
}

bool CSearchServerNetSocketData::_prepare_data(unsigned char* out_data, long len)
{
	bool bret = false;
	cJSON *root = CreateCommonJsonHead();

	cJSON *content = cJSON_CreateObject();
	cJSON_AddItemToObject(root, JSON_COMMON_CONTENT_KEY, content);
	if(content!=0)
	{
		cJSON_AddStringToObject(content, JSON_COMMON_CTYPE_KEY, _T("verify_login_reponse"));
		cJSON *pjson_value = cJSON_CreateObject();
		cJSON_AddItemToObject(content, JSON_COMMON_CVALUE_KEY, pjson_value);
		if(pjson_value!=0)
		{
			cJSON_AddNumberToObject(pjson_value, JSON_PORT_KEY, m_nport);
			cJSON_AddNumberToObject(pjson_value, JSON_NET_KIND_KEY, m_nkind);
			cJSON_AddStringToObject(pjson_value, JSON_SERVERNAME_KEY, m_strServerName.c_str());
			cJSON_AddStringToObject(pjson_value, JSON_IP_KEY, m_strIp.c_str());
		}
	}

	TCHAR *pjson_str = cJSON_Print(root);
#ifdef _UNICODE
	if(len > lstrlen(pjson_str))
	{
		char data[1024] = {0};
		int data_len = GlobalUnicodeToUtf8(pjson_str, data, 1023);
		::memcpy(out_data, data, data_len);
		bret = true;
	}

#else
	if(len > lstrlen(pjson_str)+1)
	{
		bret = true;
		::memcpy(out_data, pjson_str, lstrlen(pjson_str));
	}
#endif


	DeleteCommonJsonHead(root);

	return bret; 
}