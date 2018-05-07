#include "stdafx.h"
#include "StringIOCompletePortOverlapped.h"
#include "AppEntity.h"


CStringIOCompletePortOverlapped::CStringIOCompletePortOverlapped(CAppEntity *pShareData)
	: m_nsocket(0)
	, m_pAppEntity(pShareData)
{

}

CStringIOCompletePortOverlapped::~CStringIOCompletePortOverlapped(void)
{

}

void CStringIOCompletePortOverlapped::OverLappedOparete()
{
	if (m_pAppEntity!=NULL)
	{
		m_pAppEntity->RecevieData(m_nsocket, m_string);
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
		this->m_string = pp->m_string;
		this->m_nsocket = pp->m_nsocket;
		
		bret = true;
	}

	return bret;
}

void CStringIOCompletePortOverlapped::Update(const DWORD &nsocket, const MyString &string)
{
	m_nsocket = nsocket;
	m_string = string;
}
