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

bool CNetSocketDataParse::ParseData(const unsigned char* data, long len, CEncrypter *pEncrypter /*= 0*/){
	bool ret = false;
	
	NO_NULL(pEncrypter){
		pEncrypter->Decrypt(data, len);
	}

	if (_isType(data, len) == true)
	{
		ret = _parseData(data, len);
	}
	
	return ret;
}

void CNetSocketDataParse::RefreshOperator(COperater *operate){
	//if(operate  != 0 ){
	//	operate->Copy(this->CreateOperater());
	//}
}