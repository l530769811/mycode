#ifndef __COMMON_CSTRINGIOCOMPLETEPORTOVERLAPPED_H__
#define  __COMMON_CSTRINGIOCOMPLETEPORTOVERLAPPED_H__

#include <vector>
#include "iocompleteportoverlapped.h"
#include "MemAlloctor.h"

class CStringIOCompletePortOverlapped :
	public CIOCompletePortOverlapped
	
{
public:
	CStringIOCompletePortOverlapped(void);
	virtual ~CStringIOCompletePortOverlapped(void);

public:
	virtual void OverLappedOparete();
	virtual bool IsExit();
	virtual CIOCompletePortOverlapped* Clone();
	virtual bool Copy(const CIOCompletePortOverlapped &p);

private:
	std::wstring m_string;
};

#endif //__COMMON_CSTRINGIOCOMPLETEPORTOVERLAPPED_H__