#include "LoginAffirmNetSocketData.h"
#include "cJSON_Unicode.h"
#include "SocketProtocol.h"
#include "GlobalFunc.h"

CLoginAffirmNetSocketData::CLoginAffirmNetSocketData(int result, MyString user_name, MyString user_password)
	: m_strUserName(user_name)
	, m_strUserPassword(user_password)
	, m_result(result)
{
}


CLoginAffirmNetSocketData::~CLoginAffirmNetSocketData(void)
{
}

int CLoginAffirmNetSocketData::_prepare_data(unsigned char* out_data, long len)
{
	int bret = false;
	cJSON *root = CreateCommonJsonHead();

	cJSON *content = cJSON_CreateObject();
	cJSON_AddItemToObject(root, JSON_COMMON_CONTENT_KEY, content);
	if(content!=0)
	{
		cJSON_AddStringToObject(content, JSON_COMMON_CTYPE_KEY, _T("login_affirm_reponse"));
		cJSON *pjson_value = cJSON_CreateObject();
		cJSON_AddItemToObject(content, JSON_COMMON_CVALUE_KEY, pjson_value);
		if(pjson_value!=0)
		{
			cJSON_AddStringToObject(pjson_value, JSON_USER_NAME_KEY, m_strUserName.c_str());
			cJSON_AddStringToObject(pjson_value, JSON_USER_PASSWORD_KEY, m_strUserPassword.c_str());			
			cJSON_AddNumberToObject(pjson_value, JSON_RESULT_KEY, m_result);
		}
	}

	TCHAR *pjson_str = cJSON_Print(root);
#ifdef _UNICODE
	if(len > lstrlen(pjson_str))
	{
		char data[1024] = {0};
		int data_len = GlobalUnicodeToUtf8(pjson_str, data, 1023);
		::memcpy(out_data, data, data_len);
		bret = data_len;
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
