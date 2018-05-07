#ifndef __CINTERVALRATEOPARETOR_H__
#define __CINTERVALRATEOPARETOR_H__

#include "rateoparetor.h"
class CIntervalRateOparetor :
	public CRateOparetor
{
public:
	CIntervalRateOparetor(int sesioncount, int interval, int times, const CQxcLottery *plottery);

	virtual bool DoOparete();

	virtual ~CIntervalRateOparetor(void);

private:
	const CQxcLottery *m_phead;
	const double m_session_count;
	const int m_times;
};

#endif //