#ifndef _SINGOLEMODETEMPLETE_H__
#define _SINGOLEMODETEMPLETE_H__


///////////////////////////////
//网上摘录的单例模式-模版类;
////////////////////////////////////////

template <typename T> 
class CSingleModeTemplete
{
public:
	static T * GetInstance()
	{
		static T m_single;
		return &m_single;
	}
};
#endif //_SINGOLEMODETEMPLETE_H__