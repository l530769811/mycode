#ifndef __ESOP_CSTRINGIOCOMPLETEPORT_H__
#define __ESOP_CSTRINGIOCOMPLETEPORT_H__
#include "IOCompletePortOverlapped.h"

class CSocketRecevier;
class CStringIOCompletePortOverlapped : public CIOCompletePortOverlapped
{
public:
	CStringIOCompletePortOverlapped(CSocketRecevier *pRecevier);
	~CStringIOCompletePortOverlapped(void);

public:
	virtual void OverLappedOparete();
	virtual bool IsExit();
	virtual CIOCompletePortOverlapped* Clone();
	virtual bool Copy(const CIOCompletePortOverlapped &p);

	void Update(const DWORD &nsocket,  const BYTE *rev_buf, UINT rev_len);

private:
	BYTE  m_data[1024];
	UINT data_len;
	DWORD m_nsocket;
	CSocketRecevier *m_pRecevier;
	
};

#endif //__ESOP_CSTRINGIOCOMPLETEPORT_H__