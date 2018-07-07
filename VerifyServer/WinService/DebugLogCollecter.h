#ifndef _WINSERVICE_DEBUGLOGCOLLECTER_H__
#define _WINSERVICE_DEBUGLOGCOLLECTER_H__
#include "my_assist_define.h"
#include "LogCollecter.h"

int service_file_debug_log(MyString str_log);
int service_log(int limite, MyString str_log);

class CDebugLogCollecter : public CLogCollecter
{
	friend int service_file_debug_log(MyString str_log);
public:
	CDebugLogCollecter(void);
	virtual ~CDebugLogCollecter(void);
};

#endif _WINSERVICE_DEBUGLOGCOLLECTER_H__