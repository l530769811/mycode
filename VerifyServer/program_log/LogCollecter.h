#ifndef __PROGRAMLOG_LOGCOLLECTER_H__
#define __PROGRAMLOG_LOGCOLLECTER_H__
#include <exception>
#include "my_assist_define.h"
#include "program_log_define.h"

#define LOG_MIN_INDEX			1>>1

#define DEBUG_LOG				1<<0
#define ERROR_LOG				1<<1
#define EXCEPTION_LOG			1<<2
#define TEXT_LOG				1<<3
#define FILE_LOG				1<<4

#define LOG_MAX_INDEX			1<<15

void DoException(int limite, std::exception &e);

#define CatchException(code) \
do { \
	try \
	{ \
	code \
	} \
	catch (std::exception &e) \
	{ \
	DoException(EXCEPTION_LOG, e);\
	} \
}while(0);

typedef int (*WRITE_LOG)(MyString strLog);
int write_debug_log(MyString strLog);

class _PROGRAMLOG_IMPORT_EXPORT_DLL_ CLogCollecter
{
public:
	CLogCollecter(CLogCollecter *log_collecter);
	virtual ~CLogCollecter(void);
	friend void DoException(int limite, std::exception &e);
	bool AddLimite(int nlimite, WRITE_LOG func);
	bool RemoveLimite(int nlimite);
	bool CanLimite(int nlimite);

public:
	int Log(int limite, MyString str_log_text);

private:
	int _get_index(int limite);
	bool _add_long_func(int nlimite, WRITE_LOG func);

private:
	unsigned short m_log_type_bit;
	WRITE_LOG m_arr_log_func[16];

	static CLogCollecter *m_pThis;
};

#endif //__PROGRAMLOG_LOGCOLLECTER_H__