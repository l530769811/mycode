#include "StdAfx.h"
#include "IOCompletePortOperate.h"
#include "IOCompletePortOverlapped.h"
#include "IOCompletePort.h"

CIOCompletePortOperate::CIOCompletePortOperate(CIOCompletePortOverlapped *pOL)
	: m_pIOCompletePort(NULL)
	, m_pOL(pOL)
{
	m_pIOCompletePort = new CIOCompletePort();
	
}


CIOCompletePortOperate::~CIOCompletePortOperate(void)
{
	delete m_pIOCompletePort;
}

bool CIOCompletePortOperate::IOCompletePortPost()
{
	bool bret = false;

	if (m_pIOCompletePort!=NULL)
	{
		bret = m_pIOCompletePort->IOCompletePortPost(0, 0, m_pOL);
	}

	return bret;
}
