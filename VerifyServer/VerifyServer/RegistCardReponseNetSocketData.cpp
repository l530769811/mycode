#include "RegistCardReponseNetSocketData.h"
#include "cJSON_Unicode.h"
#include "SocketProtocol.h"
#include "GlobalFunc.h"

CRegistCardReponseNetSocketData::CRegistCardReponseNetSocketData(int result, MyString strCardNumber, MyString strCardPassword)
	: m_result(result)
	, m_strCardNumber(strCardNumber)
	, m_strCardpassword(strCardPassword)
{
}


CRegistCardReponseNetSocketData::~CRegistCardReponseNetSocketData(void)
{
}


int CRegistCardReponseNetSocketData::_prepare_data(unsigned char* out_data, long len)
{
	int bret = false;
	cJSON *root = CreateCommonJsonHead();

	cJSON *content = cJSON_CreateObject();
	cJSON_AddItemToObject(root, JSON_COMMON_CONTENT_KEY, content);
	if(content!=0)
	{
		cJSON_AddStringToObject(content, JSON_COMMON_CTYPE_KEY, _T("card_regist_reponse"));
		cJSON *pjson_value = cJSON_CreateObject();
		cJSON_AddItemToObject(content, JSON_COMMON_CVALUE_KEY, pjson_value);
		if(pjson_value!=0)
		{
			cJSON_AddNumberToObject(pjson_value, JSON_RESULT_KEY, m_result);
			cJSON_AddStringToObject(pjson_value, JSON_CARD_NUMBER_KEY, m_strCardNumber.c_str());
			cJSON_AddStringToObject(pjson_value, JSON_CARD_PASSWORD_KEY, m_strCardpassword.c_str());
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