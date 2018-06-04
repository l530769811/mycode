#include "NetSocketDataParse.h"
#include "Encrypter.h"
#include "my_assist_define.h"
#include "Operater.h"

CNetSocketDataParse::CNetSocketDataParse(void)
{
}


CNetSocketDataParse::~CNetSocketDataParse(void)
{
}

bool CNetSocketDataParse::ParseData(unsigned long socket_id, const unsigned char* data, long len, CEncrypter *pEncrypter /*= 0*/){
	bool ret = false;
	
	NO_NULL(pEncrypter){
		pEncrypter->Decrypt(data, len);
	}

	if ((ret=_isType(data, len)) == true)
	{
		_parseData(socket_id, data, len);
	}
	
	return ret;
}
