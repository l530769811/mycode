#include "ConsumeCardNetSocketDataParse.h"
#include "cJSON_Unicode.h"
#include "SocketProtocol.h"
#include "GlobalFunc.h"
#include "ConsumeCardOperater.h"

const MyString CConsumeCardNetSocketDataParse::m_strRequest = _T("consume_card_request");

CConsumeCardNetSocketDataParse::CConsumeCardNetSocketDataParse(CVIPCardManager *pmgr, CClientManager *pclient_mgr)
	: m_pmgr(pmgr)
	, m_pclient_mgr(pclient_mgr)
{
}


CConsumeCardNetSocketDataParse::~CConsumeCardNetSocketDataParse(void)
{
}


bool CConsumeCardNetSocketDataParse::_isType(const unsigned char* data, long len){
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

bool CConsumeCardNetSocketDataParse::_parseData(unsigned long socket_id, const unsigned char* data, long len){
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
			}
		}
	}

	return bret;
}

CUseCount<COperater> CConsumeCardNetSocketDataParse::CreateOperater()
{
	return CUseCount<COperater>( new CConsumeCardOperater(m_pmgr, m_pclient_mgr, m_socketid,  m_strCardNumber, m_strCardPassword));
}