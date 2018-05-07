#include "StdAfx.h"
#include "StringIOCompletePortOverlapped.h"


CStringIOCompletePortOverlapped::CStringIOCompletePortOverlapped(void)
	: m_string(_T("just test iocompleteport"))
{
}


CStringIOCompletePortOverlapped::~CStringIOCompletePortOverlapped(void)
{
}

void CStringIOCompletePortOverlapped::OverLappedOparete()
{
	wprintf(_T("iocompleteprot %s\n"), m_string.c_str());
}

bool CStringIOCompletePortOverlapped::IsExit()
{
	bool bret = false;
	
	return bret;
}

CIOCompletePortOverlapped* CStringIOCompletePortOverlapped::Clone()
{
	return new CStringIOCompletePortOverlapped(*this);
}

bool CStringIOCompletePortOverlapped::Copy(const CIOCompletePortOverlapped &p)
{
	const CStringIOCompletePortOverlapped *pp = dynamic_cast<const CStringIOCompletePortOverlapped*>(&p);
	if (pp!=NULL)
	{
		this->m_string = pp->m_string;
		return true;
	}

	return false;
}