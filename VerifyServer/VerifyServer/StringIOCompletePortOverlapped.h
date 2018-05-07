#ifndef __ESOP_CSTRINGIOCOMPLETEPORT_H__
#define __ESOP_CSTRINGIOCOMPLETEPORT_H__
#include "IOCompletePortOverlapped.h"

class CAppEntity;
class CStringIOCompletePortOverlapped : public CIOCompletePortOverlapped
{
public:
	CStringIOCompletePortOverlapped(CAppEntity *pShareData);
	~CStringIOCompletePortOverlapped(void);

public:
	virtual void OverLappedOparete();
	virtual bool IsExit();
	virtual CIOCompletePortOverlapped* Clone();
	virtual bool Copy(const CIOCompletePortOverlapped &p);

	void Update(const DWORD &nsocket, const MyString &string);

private:
	MyString m_string;
	DWORD m_nsocket;
	CAppEntity *m_pAppEntity;
	
};

#endif //__ESOP_CSTRINGIOCOMPLETEPORT_H__