#ifndef _RATEOPARETOR_H__
#define _RATEOPARETOR_H__

#include "oparetor.h"
#include "QxcLottery.h"
#include <string>

struct RateSit{
	double have_times[10];
};
struct LotteryRate
{
	const CQxcLottery *plottery_begin;
	const CQxcLottery *plottery_end;

	std::wstring sesion_section_begin;
	std::wstring sesion_section_end;
	const static int nMaxSit = 7;
	RateSit nSit[nMaxSit];
	bool bGet;
};

class CRateOparetor :
	public COparetor
{
public:
	
	CRateOparetor(int sesioncount, int interval, int times, const CQxcLottery *plottery);

	virtual bool DoOparete();
	void ToPrint();
	std::wstring toString();

	virtual ~CRateOparetor(void);


protected:
	std::wstring FormatBegainTitleToString(int num) const;
	std::wstring FormatEndToString() const;
	std::wstring FormatSitToString(int sit) const;
	std::wstring SitRateToString(int ntimes, int sit) const;

	void countRate(int ntimes, int nsit, int number);
	void remSeasonBegin(int ntimes, std::wstring strSeason);
	void remSeasonBegin(int ntimes, const CQxcLottery *p);
	void remSeasonEnd(int ntimes, std::wstring strSeason);
	void remSeasonEnd(int ntimes, const CQxcLottery *p);

private: 
	const CQxcLottery *m_phead;
	const double m_session_count;
	const int m_season_interval;
	const int m_times;
	static const int nMaxTimes = 15;
	struct LotteryRate m_rate[nMaxTimes];	
};

#endif 
