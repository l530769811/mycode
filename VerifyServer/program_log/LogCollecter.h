#ifndef __PROGRAMLOG_LOGCOLLECTER_H__
#define __PROGRAMLOG_LOGCOLLECTER_H__
#include <exception>

#define LOG_MIN_INDEX			1>>1

#define DEBUG_LOG				1<<0
#define ERROR_LOG				1<<1
#define EXCEPTION_LOG			1<<2
#define MY_EXCEPTION_LOG		1<<3
#define TEXT_LOG				1<<4

#define LOG_MAX_INDEX			1<<15

#define CatchException(code) \
do { \
	try \
	{ \
	code \
	} \
	catch (std::exception &e) \
	{ \
	;\
	} \
}while(0);

#define CatchExceptionThrow(code, exception_val_name, exception_do) \
	do { \
	try \
	{ \
	code \
	} \
	catch (std::exception &(##exception_val_name)) \
	{ \
	exception_do\
	} \
	}while(0);


class CLogCollecter
{
public:
	CLogCollecter(CLogCollecter *log_collecter);
	virtual ~CLogCollecter(void);

private:
	unsigned short m_log_type_bit;
	static CLogCollecter *m_log_collecter;
};

#endif //__PROGRAMLOG_LOGCOLLECTER_H__