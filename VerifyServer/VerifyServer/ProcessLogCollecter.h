#ifndef _VERIFYSERVER_PROCESSLOGCOLLECTER_H__
#define _VERIFYSERVER_PROCESSLOGCOLLECTER_H__
#include "LogCollecter.h"
#include <map>
#include <UseCount.h>
#include <UseCount.cpp>
#include <vector>
#include <map>
#include <queue>
#include "MemAlloctor.h"

int log(int nlimite, MyString str_log);

int write_process_log(MyString str_log);
int write_file_log(MyString str_log);



struct write_data;
class CCommunicateData;
class CLocalCommunication;

struct write_data /*: public CMemAlloctor*/{
	write_data():nlen(0){}
	static const int NLEN = 1024;
	unsigned char data[1024];
	int nlen;
	virtual int WriteData(unsigned char *data, int len)=0;
};

struct communication_write_data : public write_data{
	communication_write_data(CLocalCommunication *pcommunicater);
	CLocalCommunication *m_pcommunicater;
	virtual int WriteData(unsigned char *data, int len);
};

struct file_write_data : public write_data{
	file_write_data(MyString str_log);
	MyString m_str_log;
	virtual int WriteData(unsigned char *data, int len);
};

class CProcessLogCollecter :
	public CLogCollecter
{
	friend int write_process_log(MyString str_log);
	friend int write_file_log(MyString str_log);
public:
	CProcessLogCollecter(void);
	virtual ~CProcessLogCollecter(void);
	void CommunicateData(CCommunicateData &data);

private:
	static unsigned int __stdcall _CommunicationThreadProc(void * pParam);
	bool PostWrite(write_data *pdata);

private:
	CLocalCommunication *m_pcommunicater;
	HANDLE m_hcommunication_thread;
	bool m_bthreadRunning;
	std::vector<CCommunicateData*> m_data_vector;
	std::map<CCommunicateData*, CLocalCommunication*> m_map_allow_communicate;

	std::queue<write_data*> m_write_queue;
	CRITICAL_SECTION m_criQueueLock;
	HANDLE m_hEvent;
};


#endif _VERIFYSERVER_PROCESSLOGCOLLECTER_H__