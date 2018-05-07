#ifndef __COMMON_CIOCOMPLETEPORTOVERLAPPED_H__
#define __COMMON_CIOCOMPLETEPORTOVERLAPPED_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include "MemAlloctor.h"
#include "define.h"

class _IO_IMPORT_EXPORT_DLL_ CIOCompletePortOverlapped : public CMemAlloctor
{
	friend class CIOCompletePort;
public:
	CIOCompletePortOverlapped(void);
	virtual ~CIOCompletePortOverlapped(void)=0;

public:
	virtual void OverLappedOparete() = 0;
	virtual bool IsExit() = 0;
	virtual CIOCompletePortOverlapped* Clone() = 0;
	virtual bool Copy(const CIOCompletePortOverlapped &p) = 0;

private:
	OVERLAPPED ol;
};

#endif //__COMMON_CIOCOMPLETEPORTOVERLAPPED_H__