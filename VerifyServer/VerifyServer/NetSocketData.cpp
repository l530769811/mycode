#include "NetSocketData.h"
#include "NetSocketProxy.h"

CNetSocketData::CNetSocketData(void)
{
}


CNetSocketData::~CNetSocketData(void)
{
}

int CNetSocketData::BeSend(CNetSocketProxy *pProxy, CEncrypter *pEncrypter /* = 0 */){
	int nret = -1;

	unsigned char send_data[1024] = {0};
	int len = _prepare_data(send_data, 1023);

	if (pProxy!=0 && len>0)
	{
		pProxy->SendData(send_data, len);
	}

	return nret;
}
