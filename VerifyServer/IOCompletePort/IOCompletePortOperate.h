#ifndef __COMMON_CIOCOMPLETEPORTOPERATE_H__
#define __COMMON_CIOCOMPLETEPORTOPERATE_H__

#include "define.h"

class CIOCompletePort;
class CIOCompletePortOverlapped;
class _IO_IMPORT_EXPORT_DLL_ CIOCompletePortOperate
{
public:
	CIOCompletePortOperate(CIOCompletePortOverlapped *pOL);
	~CIOCompletePortOperate(void);

public:
	bool IOCompletePortPost();

private:
	CIOCompletePort *m_pIOCompletePort;
	CIOCompletePortOverlapped *m_pOL;
};

#endif //__COMMON_CIOCOMPLETEPORTOPERATE_H__