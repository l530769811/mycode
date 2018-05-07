#include "StdAfx.h"
#include "IntervalRateOparetor.h"


CIntervalRateOparetor::CIntervalRateOparetor(int sesioncount, int interval, int times, const CQxcLottery *plottery)
	: CRateOparetor(sesioncount, interval, times, plottery),
	m_phead(plottery),
	m_session_count(sesioncount),
	m_times(times)
{
}


CIntervalRateOparetor::~CIntervalRateOparetor(void)
{
}

bool CIntervalRateOparetor::DoOparete()
{
	bool ret = false;

	if (m_phead != NULL)
	{
		const CQxcLottery *plist = m_phead;
		int ncount = 0;

		int timescount = 0;
		const int sit_max = 7;

		while(lottery_list_is_last(plist, m_phead) != true)
		{
			const  CQxcLottery *phead = NULL;
			if(timescount >= m_times)
			{
				break;
			}

			plist = lottery_list_get_next(plist);
			phead = plist;
			while(lottery_list_is_last(phead, m_phead) != true){
				if (phead != NULL)
				{

					if (ncount >= m_session_count)
					{
						ncount = 0;
						timescount++;
						break;
					}

					if (ncount <= 0)
					{
						//season range end;

						//if(!lottery_list_is_first(plist, m_phead))
						{
							remSeasonEnd(timescount, phead);
							remSeasonEnd(timescount, phead->getSeason());
						}
					}

					if (ncount >= m_session_count-1)
					{
						//season range begin;
						if(!lottery_list_is_first(phead, m_phead))
						{
							remSeasonBegin(timescount, phead);
						}
						remSeasonBegin(timescount, phead->getSeason());
					}

					ncount++;

					for(int nsit=0; nsit<sit_max; nsit++)
					{
						int number = phead->getNumber(nsit+1);
						countRate(timescount, nsit, number);
					}
					phead = lottery_list_get_next(phead);
				}
			}

		}
	}

	return ret;
}