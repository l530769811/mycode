#ifndef _CLOTTERYFILER_H__
#define  _CLOTTERYFILER_H__
#include <string>
#include "QxcLottery.h"


class CLotteryFiler
{
public:
	CLotteryFiler(void);
	~CLotteryFiler(void);

	void read_lottery();
	void write_lottery();
	
	const CQxcLottery* get_lottery_head() const;

private:
	static std::wstring strFileName;
	CQxcLottery *m_phead;
};

#endif

