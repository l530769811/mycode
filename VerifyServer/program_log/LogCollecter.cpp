#include "LogCollecter.h"


CLogCollecter::CLogCollecter(CLogCollecter *log_collecter)
{
	CLogCollecter::m_log_collecter = log_collecter;
	//CatchException(if(0){});
	CatchExceptionThrow(if (0){}, e, if (0)	{});
	
}


CLogCollecter::~CLogCollecter(void)
{
}
