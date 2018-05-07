#include "StdAfx.h"
#include "RateOparetor.h"
//#include "QxcLottery.h"

#include <iostream>
#include <sstream>
#include <iomanip>


CRateOparetor::CRateOparetor(int sesionCount, int interval, int times,  const CQxcLottery *plottery)
	: m_session_count(sesionCount),
	m_season_interval(interval),
	m_times(times),
	m_phead(plottery)
{
	::memset(m_rate, 0, sizeof(LotteryRate)*nMaxTimes);
	//for (int times=0; times<m_times; times++)
	//{
	//	for (int nsit=0; nsit<LotteryRate::nMaxSit; nsit++)
	//	{

	//		for (int have_times=0; have_times<10; have_times++)
	//		{
	//			m_rate[times].nSit[nsit].have_times[have_times] = 0.1f;
	//		}
	//	}
	//}
}


CRateOparetor::~CRateOparetor(void)
{
}

void CRateOparetor::countRate(int ntimes, int nsit, int number)
{
	if (number>=0 && number<=9)
		(this->m_rate[ntimes].nSit[nsit].have_times[number])++;
}

void CRateOparetor::remSeasonBegin(int ntimes, const CQxcLottery *p)
{
	this->m_rate[ntimes].plottery_begin = p;
}

void CRateOparetor::remSeasonBegin(int ntimes, std::wstring strSeason)
{
	this->m_rate[ntimes].sesion_section_begin = strSeason;
}
 
void CRateOparetor::remSeasonEnd(int ntimes, const CQxcLottery *p)
{
	this->m_rate[ntimes].plottery_end = p;
}

void CRateOparetor::remSeasonEnd(int ntimes, std::wstring strSeason)
{
	this->m_rate[ntimes].sesion_section_end = strSeason;
}

bool CRateOparetor::DoOparete()
{
	bool bret = false;
	if (m_phead != NULL)
	{
		const CQxcLottery *plist = m_phead;
		int count = 0;
		int interval_count = 0;

		int timescount = 0;
		const int sit_max = 7;

		while(lottery_list_is_last(plist, m_phead) != true)
		{
			if(timescount >= m_times)
			{
				break;
			}

			plist = lottery_list_get_next(plist);
			if (plist!=NULL)
			{
			
				if(count <= 0 && interval_count <= m_session_count)
				{
					//end of lottery 
					remSeasonEnd(timescount, plist);
					remSeasonEnd(timescount, plist->getSeason());
				}
				
				if(count>=this->m_session_count-1 )
				{
					//begin of lottery
					if(!lottery_list_is_first(plist, m_phead))
						remSeasonBegin(timescount, plist);
					
					remSeasonBegin(timescount, plist->getSeason());				
					
				}
				count++;
				interval_count++;

				if (interval_count >  m_session_count && interval_count < m_season_interval)
				{
					
					continue;
				}

				if (interval_count > m_season_interval)
				{
					timescount++;
					interval_count = 0;
					count = 0;
				}


				for(int nsit=0; nsit<sit_max; nsit++)
				{
					int ntemp = plist->getNumber(nsit+1);
					countRate(timescount, nsit, ntemp);
				}	
			}
		}
#ifdef _DEBUG 
		std::cout << count << std::endl;
#endif
		
	}
	return bret;
}

void CRateOparetor::ToPrint()
{
	std::wcout << std::setiosflags(std::ios::fixed) << std::setprecision(2) << toString() << std::endl;
}

std::wstring times_string[] = {
	_T("one"), _T("two"), _T("three"),_T("four"), _T("five"),_T("six"), _T("seven"), _T("eight"), _T("nine"), _T("ten")
};

std::wstring sit_string[] = {
	_T("[[1]]"), _T("[[2]]"), _T("[[3]]"), _T("[[4]]"), _T("[[5]]"), _T("[[6]]"), _T("[[7]]")
};

std::wstring number_string[] = {
	_T("(0)"),
	_T("(1)"),
	_T("(2)"),
	_T("(3)"),
	_T("(4)"),
	_T("(5)"),
	_T("(6)"),
	_T("(7)"),
	_T("(8)"),
	_T("(9)")
};

std::wstring CRateOparetor::FormatBegainTitleToString(int num) const
{
	std::wostringstream strOstream;	
	const int char_times = 40;
	int times=0;

	if (num > 0)
	{
		times = num;
	}

	strOstream << std::endl;
	for (int i=0; i<char_times; i++ )
	{
		strOstream << _T("-");
	}
	/*strOstream << _T("---------------------------------");*/
	//s = s + strOstream.str();
	for(int j=0; j<(8-wcslen(times_string[times].c_str() ))/2; j++)
		strOstream << _T(" ");
	strOstream << times_string[times];
	for(int j=0; j<(8-wcslen(times_string[times].c_str() ))/2; j++)
		strOstream << _T(" ");
	for (int i=0; i<char_times; i++ )
	{
		strOstream << _T("-");
	}

	std::wstring next_lottery;
	if (m_rate[times].plottery_end != NULL)
	{
		const CQxcLottery *pnext = lottery_list_get_prev(m_rate[times].plottery_end);
		std::wostringstream s;
		s << pnext->getNumber(1) << _T(" ");
		s << pnext->getNumber(2) << _T(" ");
		s << pnext->getNumber(3) << _T(" ");
		s << pnext->getNumber(4) << _T(" ");
		s << pnext->getNumber(5) << _T(" ");
		s << pnext->getNumber(6) << _T(" ");
		s << pnext->getNumber(7) << _T(" ");
		next_lottery = s.str();
	}

	std::wstring prev_lottery;
	if (m_rate[times].plottery_begin != NULL)
	{
		const CQxcLottery *pprev = lottery_list_get_next(m_rate[times].plottery_begin);
		std::wostringstream s;
		s << pprev->getNumber(1) << _T(" ");
		s << pprev->getNumber(2) << _T(" ");
		s << pprev->getNumber(3) << _T(" ");
		s << pprev->getNumber(4) << _T(" ");
		s << pprev->getNumber(5) << _T(" ");
		s << pprev->getNumber(6) << _T(" ");
		s << pprev->getNumber(7) << _T(" ");
		prev_lottery = s.str();
	}


	std::wstring season_range = m_rate[times].sesion_section_begin + _T("~~") +  m_rate[times].sesion_section_end 
		+ _T("\t") + _T("next_lottery:") + next_lottery + _T("\t") + _T("prev_lottery:") + prev_lottery;
	strOstream << std::endl;
	int i1 = (wcslen(season_range.c_str()))/2;
	for (int i=0; i<char_times-i1 ; i++ )
	{
		strOstream << _T(" ");
	}
	strOstream << season_range;
	strOstream << std::endl;

	return strOstream.str();
}


std::wstring CRateOparetor::FormatEndToString() const
{
	//end format

	const int char_times = 40;
	std::wostringstream strOstream;

	//s = s + _T("\t") + sit_string[i] + _T("\t");
	strOstream << std::endl;
	for (int i=0; i<char_times; i++ )
	{
		strOstream << _T("-");
	}
	strOstream << _T("-------");
	for (int i=0; i<char_times; i++ )
	{
		strOstream << _T("-");
	}
	strOstream << std::endl;
	strOstream << std::endl;

	return strOstream.str();
}

std::wstring CRateOparetor::FormatSitToString(int sit) const
{
	std::wostringstream strOstream;
	strOstream << _T("\t");
	for (int i=0; i<10; i++)
	{
		strOstream << number_string[i] << _T("\t  ");
	}

	strOstream << std::endl;
	strOstream << sit_string[sit];

	return strOstream.str();
}

std::wstring CRateOparetor::SitRateToString(int ntimes, int nsit) const
{
	std::wostringstream strOstream;	
	const int char_times = 34;
	int times=0;
	if ( ntimes>0 )
	{
		times = ntimes;
	}

	strOstream <<_T("\t");
	for (int i=0; i<10; i++)
	{
		strOstream << std::setiosflags(std::ios::fixed) << std::setprecision(2) << ((float)((m_rate[times].nSit[nsit].have_times[i])/m_session_count)*100) << _T("%") << _T("\t");
	}
	strOstream << std::endl;
	strOstream << std::endl;

	return strOstream.str();
}

std::wstring CRateOparetor::toString()
{
	std::wstring s;
	
	std::wostringstream strOstream;
	strOstream << std::endl;
	s = s + strOstream.str();

	for (int times=0; times<m_times; times++)
	{
		s = s + this->FormatBegainTitleToString(times);
		//begin format
		for(int nsit=0; nsit<LotteryRate::nMaxSit; nsit++)
		{
			

			s = s + this->FormatSitToString(nsit);
			s = s + this->SitRateToString(times, nsit);

			
		}
		//end format
		s = s + this->FormatEndToString();
		
	}
	
	return s;
}
