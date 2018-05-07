#ifndef _COMMON_CMEMALLOCTOR_H__
#define _COMMON_CMEMALLOCTOR_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <vector>
#include <list>

class CMemPool;
class CMem
{
	friend CMemPool;
public:
	CMem(void);
	CMem(size_t nsize);
	~CMem(void);


public:
	bool IsFree();
	size_t GetSize() const;


private:
	bool _UseMem(size_t nsize);
	bool _UnUseMem();
	void* _GetBuff();
	void _NewBuf(size_t nsize);
	void _DeleteBuf();

private:
	CMem& operator=(const CMem& mem);
	CMem(const CMem& mem); 

private:
	static const int nTenKB = 1024*10;
	unsigned char  m_byte[nTenKB];
	bool m_bFree;
	size_t m_nSize;
	
};

class CMemPool
{
public:
	CMemPool(void);
	virtual ~CMemPool(void);

public:
	static void* NewMem(size_t nsize);
	static void  DeleteMem(void *p);

private:
	void _initMemBuf();
	void _clearMemBuf();

private:
	static const int nInitMaxCount = 100;
	static const int nTenKB = 1024*10;

	static std::vector<CMem*> m_vecMemBuf;
	static std::vector<CMem*> m_vecOtherMembuf;

	static std::list<CMem*> m_listFreeMem;
	static std::list<CMem*> m_listBusyMem;
	static CRITICAL_SECTION m_csMemLock;
};

class CMemAlloctor
{
public:
	CMemAlloctor(void);
	virtual ~CMemAlloctor(void);

public:
	void* operator new(size_t nsize);
	void  operator delete(void *p);

private:
	static CMemPool memPool;
};

#endif //_COMMON_CMEMALLOCTOR_H__