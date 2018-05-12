#include "stdafx.h"
#include "StringIOCompletePortOverlapped.h"
#include "AppEntity.h"
#include "SocketRecevier.h"


CStringIOCompletePortOverlapped::CStringIOCompletePortOverlapped(CSocketRecevier *pRecevier)
	: m_nsocket(0)
	, m_pRecevier(pRecevier)
{
	::memset(m_data, 0, 1024);
	data_len = 0;
}

CStringIOCompletePortOverlapped::~CStringIOCompletePortOverlapped(void)
{

}

void CStringIOCompletePortOverlapped::OverLappedOparete()
{
	if (m_pRecevier!=NULL)
	{
		m_pRecevier->Recevie(m_nsocket, m_data, data_len);
	}
		
}

bool CStringIOCompletePortOverlapped::IsExit()
{
	bool bret = false;

	return bret;
}

CIOCompletePortOverlapped* CStringIOCompletePortOverlapped::Clone()
{
	CIOCompletePortOverlapped *p = NULL;
	p = new CStringIOCompletePortOverlapped(*this);
	return p;
}

bool CStringIOCompletePortOverlapped::Copy(const CIOCompletePortOverlapped &p)
{
	bool bret = false;
	
	const CStringIOCompletePortOverlapped *pp = dynamic_cast<const CStringIOCompletePortOverlapped*>(&p);
	if (pp!=NULL)
	{
		this->Update(pp->m_nsocket, pp->m_data, pp->data_len);
	}

	return bret;
}

void CStringIOCompletePortOverlapped::Update(const DWORD &nsocket,  const BYTE *rev_buf, UINT rev_len)
{
	m_nsocket = nsocket;
	if(rev_len <= 1024)
		data_len = rev_len;
	else
		data_len = 1024;

	::memcpy(m_data, rev_buf, data_len);
}
