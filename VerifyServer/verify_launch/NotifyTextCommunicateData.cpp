#include "NotifyTextCommunicateData.h"
#include "cJSON_Unicode.h"
#include "SocketProtocol.h"
#include "GlobalFunc.h"

const MyString CNotifyTextCommunicateData::m_strRequest = _T("communicate_notify_text");
CNotifyTextCommunicateData::CNotifyTextCommunicateData(MyString strNotifyText)
	: m_strNotifyText(strNotifyText)
{
}

CNotifyTextCommunicateData::CNotifyTextCommunicateData(void)
{
}


CNotifyTextCommunicateData::~CNotifyTextCommunicateData(void)
{
}

bool CNotifyTextCommunicateData::IsType(const unsigned char *data, const int &data_len)
{
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

int CNotifyTextCommunicateData::ComposeData(const unsigned char *data, const int &data_len)
{
	int nret = -1;
	
	TCHAR unicode[1024] = {0};
	GlobalUtf8ToUnicode((const char*)data, unicode, 1023);
	cJSON *proot = cJSON_Parse(unicode);
	if(proot!=0){
		cJSON *pcontent = cJSON_GetObjectItem(proot, JSON_COMMON_CONTENT_KEY);
		if (pcontent!=0)
		{
			cJSON *pvalue = cJSON_GetObjectItem(pcontent, JSON_COMMON_CVALUE_KEY);
			if(pvalue!=0 && pvalue->type == cJSON_Object){
				cJSON *p = cJSON_GetObjectItem(pvalue, _T("notify_text"));
				if (p!=0 && p->type==cJSON_String)
				{
					m_strNotifyText = p->valuestring;
				}
			}	
			nret = data_len;
		}
	}

	return nret;
}

int CNotifyTextCommunicateData::DecomposeData(unsigned char *data, int &data_len) const
{
	int nret = -1;
	cJSON *root = CreateCommonJsonHead();

	cJSON *content = cJSON_CreateObject();
	cJSON_AddItemToObject(root, JSON_COMMON_CONTENT_KEY, content);
	if(content!=0)
	{
		cJSON_AddStringToObject(content, JSON_COMMON_CTYPE_KEY, m_strRequest.c_str());
		cJSON *pjson_value = cJSON_CreateObject();
		cJSON_AddItemToObject(content, JSON_COMMON_CVALUE_KEY, pjson_value);
		if(pjson_value!=0)
		{
			cJSON_AddStringToObject(pjson_value, _T("notify_text"), m_strNotifyText.c_str());			
		}
	}

	TCHAR *pjson_str = cJSON_Print(root);
#ifdef _UNICODE
	if(data_len > lstrlen(pjson_str))
	{
		char data_data[1024] = {0};
		int data_len = GlobalUnicodeToUtf8(pjson_str, data_data, 1023);
		::memcpy(data, data_data, data_len);
		nret = data_len;
	}

#else
	if(len > lstrlen(pjson_str)+1)
	{
		nret = lstrlen(pjson_str)+1;
		::memcpy(out_data, pjson_str, lstrlen(pjson_str));
	}
#endif
	DeleteCommonJsonHead(root);

	return nret;
}

MyString CNotifyTextCommunicateData::toString() const
{
	return m_strNotifyText;
}