#include "UseCount.h"
//
//template <typename T>
//CUseCount<T>::CUseCount(void)
//	: m_pCount(new atomic_t(0))
//	, p(0)
//{
//
//}
//
//template <typename T>
//CUseCount<T>::CUseCount(T* pp)
//	: m_pCount(new atomic_t(1))
//	, p(pp)
//{
//	ATOMIC_INIT(1);
//}
//template <typename T>
//CUseCount<T>::CUseCount(const CUseCount<T> &u)
//	:m_pCount(u.m_pCount)
//{
//	//CLock lock(&m_lock);
//	atomic_inc(m_pCount);//(*m_pCount)++;//(*u.m_pCount) also ++;
//	this->p = u.p;
//
//}
//template <typename T>
//CUseCount<T>& CUseCount<T>::operator=(const CUseCount &u)
//{
//	//CLock lock(&m_lock);
//	//为什么不先判断是否复制同一个CUseCount?;
//
//	//atomic_inc((long*)(u.m_pCount));//(*u.m_pCount)++;	
//	//atomic_dec((long*)(u.m_pCount));//--(*m_pCount);
//
//	//if ((*m_pCount) <= 0)
//	//	delete m_pCount;
//	//
//	//atomic_set((unsigned  long*)(&m_pCount), (long)u.m_pCount);//m_pCount = u.m_pCount;
//	if(Reattach(u))
//	{
//		delete p;
//		p = 0;
//	}
//
//	p = u.p;
//	return *this;
//}
//
//
//template <typename T>
//CUseCount<T>::~CUseCount(void)
//{
//	if(Only())
//	{
//		delete p;
//		p = 0;
//
//		delete m_pCount;
//		m_pCount = 0;
//	}
//	else
//	{
//		atomic_dec((long*)m_pCount);//--(*m_pCount);
//	}
//
//}
//
//template <typename T>
//bool CUseCount<T>::isNull() const
//{
//	bool bret = false;
//	if(p==0)
//	{
//		bret = true;
//	}
//	return bret;
//}
//
//template <typename T>
//bool CUseCount<T>::Only()
//{	
//	return (*m_pCount) == 1;
//}
//
//template <typename T>
//bool	CUseCount<T>::Reattach(const CUseCount<T>& u)
//{
//
//	atomic_inc((long*)u.m_pCount);  //++(*u.m_pCount);
//	atomic_dec((long*)m_pCount);//--(*m_pCount);
//
//	if ((*m_pCount) == 0)
//	{
//		delete m_pCount;
//
//		atomic_set((unsigned  long*)(&m_pCount), (long)u.m_pCount);//m_pCount = u.m_pCount;
//		return true;
//	}
//
//	atomic_set((unsigned  long*)(&m_pCount), (long)u.m_pCount);//m_pCount = u.m_pCount;
//	return false;
//}
//
//template <typename T>
//bool CUseCount<T>::Makeonly()
//{
//
//	if (*m_pCount == 1)
//		return false;
//
//	atomic_dec((long*)(m_pCount));//--(*m_pCount);
//	m_pCount = new atomic_t(1);
//
//	return true;
//}
//
//template <typename T>
//T* CUseCount<T>::operator->()
//{
//	return p;
//}

