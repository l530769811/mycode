#ifndef _SINGOLEMODETEMPLETE_H__
#define _SINGOLEMODETEMPLETE_H__
#include "common_define.h"

///////////////////////////////
//网上摘录的单例模式-模版类;
////////////////////////////////////////

template <typename T> 
class _COMMON_IMPORT_EXPORT_DLL_ CSingleModeTemplete
{
public:
	static T * GetInstance()
	{
		static T m_single;
		return &m_single;
	}
};
#endif //_SINGOLEMODETEMPLETE_H__