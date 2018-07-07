#include "DebugLogCollecter.h"
#include "ServiceLogFile.h"

CDebugLogCollecter service_loger;
int service_log(int limite, MyString str_log)
{
	return service_loger.Log(limite, str_log);	 
}

int service_file_debug_log(MyString str_log)
{
	int nret = -1;
	CServiceLogFile log_file;

	nret = log_file.WriteLog(str_log);
	return nret;
}

CDebugLogCollecter::CDebugLogCollecter(void)
	: CLogCollecter(this)
{
	this->AddLimite(FILE_LOG, service_file_debug_log);
}


CDebugLogCollecter::~CDebugLogCollecter(void)
{
}
