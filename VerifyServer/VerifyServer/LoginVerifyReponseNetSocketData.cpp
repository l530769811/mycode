#include "LoginVerifyReponseNetSocketData.h"
#include "cJSON_Unicode.h"
#include "SocketProtocol.h"

CLoginVerifyReponseNetSocketData::CLoginVerifyReponseNetSocketData(int result)
	: m_result(result)
{
}


CLoginVerifyReponseNetSocketData::~CLoginVerifyReponseNetSocketData(void)
{
}

bool CLoginVerifyReponseNetSocketData::_prepare_data(const unsigned char* out_data, long len)
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
			cJSON_AddNumberToObject(pjson_value, JSON_RESULT_KEY, m_result);
		}
	}

	TCHAR *pjson_str = cJSON_Print(root);
#ifdef _UNICODE
	if(len > lstrlen(pjson_str)*2+1)
	{
		::memcmp(out_data, pjson_str, lstrlen(pjson_str)*2);
		bret = true;
	}
	
#else
	if(len > lstrlen(pjson_str)+1)
	{
		bret = true;
		::memcmp(out_data, pjson_str, lstrlen(pjson_str));
	}
#endif
	

	DeleteCommonJsonHead(root);

	return bret; 
}
