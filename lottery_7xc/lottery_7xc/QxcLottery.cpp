#include "StdAfx.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include "QxcLottery.h"
#include "RateOparetor.h"

CQxcLottery::CQxcLottery(void)
	: m_i1(0),
	m_i2(0),
	m_i3(0),
	m_i4(0),
	m_i5(0),
	m_i6(0),
	m_i7(0)
{
}

CQxcLottery::CQxcLottery(std::wstring sesion, int i1, int i2, int i3, int i4)
	: m_strsesion(sesion), 
	m_i1(i1),
	m_i2(i2),
	m_i3(i3),
	m_i4(i4),
	m_i5(0),
	m_i6(0),
	m_i7(0)

{

}

CQxcLottery::CQxcLottery(std::wstring sesion, int i1, int i2, int i3, int i4, int i5, int i6, int i7)
	: m_strsesion(sesion), 
	m_i1(i1),
	m_i2(i2),
	m_i3(i3),
	m_i4(i4),
	m_i5(i5),
	m_i6(i6),
	m_i7(i7)

{

}


CQxcLottery::~CQxcLottery(void)
{
}

int CQxcLottery::get4Total() const 
{
	return m_i1 + m_i2 
		+ m_i3 + m_i4; 
}

int CQxcLottery::getAllTotal() const
{
	return m_i1 + m_i2 
		+ m_i3 + m_i4 + m_i5 + m_i6 + m_i7; 
}

//return 0 if find out ,or return -1
int CQxcLottery::find(int number, int sit) const
{
	int ret = -1;

	switch(sit)
	{
	case 1:
		if(number == m_i1)
			ret = 0;
		break;
	case 2:
		if(number == m_i2)
			ret = 0;
		break;
	case 3:
		if(number == m_i3)
			ret = 0;
		break;
	case 4:
		if(number == m_i4)
			ret = 0;
		break;
	case 5:
		if(number == m_i5)
			ret = 0;
		break;
	case 6:
		if(number == m_i6)
			ret = 0;
		break;
	case 7:
		if(number == m_i7)
			ret = 0;
		break;

	default:
		;
	}

	return ret;
}

int CQxcLottery::getNumber(int sit) const
{
	int ret = -1;

	switch(sit)
	{
	case 1:
		return m_i1;
	case 2:
		return m_i2;
	case 3:
		return m_i3;
	case 4:
		return m_i4;
	case 5:
		return m_i5;
	case 6:
		return m_i6;
	case 7:
		return m_i7;

	default:
		;
	}

	return ret;
}
 std::wstring CQxcLottery::getSeason() const 
 {
	 return m_strsesion;
 }

void lottery_list_init(CQxcLottery *phead)
{
	phead->m_list.next = &phead->m_list;
	phead->m_list.prev= &phead->m_list;
}


static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;

}

bool lottery_list_del(CQxcLottery *pdel)
{
	bool bret = false;
	if (pdel != NULL)
	{
		//phead->m_list.prev->next = phead->m_list.next;
		//phead->m_list.next->prev =  phead->m_list.prev;
		
		__list_del(pdel->m_list.prev, pdel->m_list.next);
		pdel->m_list.prev = NULL;
		pdel->m_list.next = NULL;

		delete pdel;
		pdel = NULL;

		bret = true;
	}
	return bret;
}

static inline void __list_add(struct list_head * pnew,
struct list_head *prev, 
struct list_head *next)
{
	next->prev = pnew;
	pnew->next = next;
	pnew->prev = prev;
	prev->next = pnew;
}

bool lottery_list_add(CQxcLottery *pnew, CQxcLottery *phead)
{
	bool bret = false;
	if (pnew!=NULL && phead!=NULL)
	{
		__list_add(&pnew->m_list, &phead->m_list, phead->m_list.next);
		bret = true;
	}
	else if(pnew!=NULL && phead==NULL)//m_list head
	{
		__list_add(&pnew->m_list, &pnew->m_list, &pnew->m_list);
		bret = true;
	}
	else
	{
		;
	}

	return bret;
}

void lottery_list_add_tail(CQxcLottery *pnew, CQxcLottery *phead)
{
	__list_add(&pnew->m_list, phead->m_list.prev, &phead->m_list);
}

void create_lottery(CQxcLottery *phead, std::wstring sesion, int i1, int i2, int i3, int i4, bool tail /*= true*/)
{
	CQxcLottery *pnew = new CQxcLottery(sesion,	i1, i2, i3, i4);

	if (tail)
		lottery_list_add_tail(pnew, phead);
	else
		lottery_list_add(pnew, phead);
}

void create_lottery(CQxcLottery *phead, std::wstring sesion, int i1, int i2, int i3, int i4, int i5, int i6, int i7, bool tail /*= true*/)
{
	CQxcLottery *pnew = new CQxcLottery(sesion,	i1, i2, i3, i4, i5, i6, i7);

	if (tail)
		lottery_list_add_tail(pnew, phead);
	else
		lottery_list_add(pnew, phead);
}

void free_lottery(CQxcLottery *phead)
{
	struct list_head *ppos = NULL;
	CQxcLottery *pdel  = NULL;
	list_for_each(ppos, &phead->m_list)
	{
		//CQxcLottery *pdel= container_of(ppos, CQxcLottery, m_list);
		pdel = CONTAINING_RECORD(ppos, CQxcLottery, m_list);
		ppos = pdel->m_list.next;
		lottery_list_del(pdel);
	}
	
}

const CQxcLottery* lottery_list_get_next(const CQxcLottery *phead)
{
	return  CONTAINING_RECORD(phead->m_list.next, CQxcLottery, m_list);
}
const CQxcLottery* lottery_list_get_prev(const CQxcLottery *phead)
{
	return CONTAINING_RECORD(phead->m_list.prev, CQxcLottery, m_list);
}