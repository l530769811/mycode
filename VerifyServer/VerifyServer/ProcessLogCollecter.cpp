#include "ProcessLogCollecter.h"

#include "LocalCommunication.h"
#include "NotifyTextCommunicateData.h"
#include "CommunicateData.h"
#include <iterator>
#include <process.h>
#include "LogFile.h"

CProcessLogCollecter log_collecter;

int log(int nlimite, MyString str_log)
{
	return log_collecter.Log(nlimite, str_log);
}


int write_process_log(MyString str_log)
{
	unsigned char log_ch[1024] = {0};

	CNotifyTextCommunicateData data(str_log);

	unsigned char writedata[1024] = {0};
	int nlen = 1023;
	nlen = data.DecomposeData(writedata, nlen);	

	write_data *pwritedata = new communication_write_data(log_collecter.m_pcommunicater);
	::memcpy(pwritedata->data, writedata, nlen);
	pwritedata->nlen = nlen;

	if(log_collecter.PostWrite(pwritedata))
	{
		return nlen;
	}

	return -1;
}

int write_file_log(MyString str_log)
{
	int nret  = -1;
	write_data *pwritedata = new file_write_data(str_log);
	if(log_collecter.PostWrite(pwritedata))
	{
		nret = lstrlen(str_log.c_str());
	}
	return nret;
}


CProcessLogCollecter::CProcessLogCollecter(void)
	: CLogCollecter(this)
	, m_hcommunication_thread(INVALID_HANDLE_VALUE)
	, m_bthreadRunning(false)
	, m_hEvent(INVALID_HANDLE_VALUE)
{
	this->AddLimite(TEXT_LOG, write_process_log);
	this->AddLimite(FILE_LOG, write_file_log);
	m_pcommunicater = new CLocalCommunication(_T("verifyserver"), 1);

	m_data_vector.push_back(new CNotifyTextCommunicateData);
	::InitializeCriticalSection(&m_criQueueLock);
	m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if(m_hcommunication_thread==INVALID_HANDLE_VALUE || m_hcommunication_thread==0)
	{
		m_bthreadRunning = true;
		m_hcommunication_thread = (HANDLE)_beginthreadex(NULL, 0, _CommunicationThreadProc, reinterpret_cast<void *>(this), 0, 0);
		if(m_hcommunication_thread==INVALID_HANDLE_VALUE || m_hcommunication_thread==0)
		{
			m_bthreadRunning = false;
		}
	}
}


CProcessLogCollecter::~CProcessLogCollecter(void)
{
	if (m_hcommunication_thread!=NULL && m_hcommunication_thread!=INVALID_HANDLE_VALUE && m_bthreadRunning==true)
	{
		if(m_pcommunicater!= 0){
			m_pcommunicater->DisconnectedNamePipe();
		}

		m_bthreadRunning = false;
		::WaitForSingleObject(m_hcommunication_thread, INFINITE);	
		::CloseHandle(m_hcommunication_thread);
		m_hcommunication_thread = INVALID_HANDLE_VALUE;
	}

	::DeleteCriticalSection(&m_criQueueLock);
	if (m_hEvent!=NULL && m_hEvent!=INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hEvent);
	}

	SAFE_DELETE(m_pcommunicater);
	for(std::vector<CCommunicateData*>::iterator it_begin=m_data_vector.begin(); it_begin!=m_data_vector.end(); it_begin++)
	{
		CCommunicateData *pdata = *it_begin;
		delete pdata;
	}
}


bool CProcessLogCollecter::PostWrite(write_data *data)
{
	bool ret = false;
	if(m_bthreadRunning == true && data!=0 && m_write_queue.size() < 50){
		::EnterCriticalSection(&m_criQueueLock);		
		m_write_queue.push(data);
		::LeaveCriticalSection(&m_criQueueLock);
		::SetEvent(m_hEvent);
		ret = true;
	}

	return ret;
}

void CProcessLogCollecter::CommunicateData(CCommunicateData &data)
{
	CLocalCommunication *p = 0;
	try{
		p=m_map_allow_communicate.at(&data);
	}
	catch(std::out_of_range e)
	{
		p = 0;	
	}

	if(p==0)
	{
		m_map_allow_communicate.insert(std::map<CCommunicateData*, CLocalCommunication*>::value_type(&data, m_pcommunicater));
	}
}

unsigned int CProcessLogCollecter::_CommunicationThreadProc(void * pParam)
{
	int ret = -1;
	CProcessLogCollecter *pApp = reinterpret_cast<CProcessLogCollecter*>(pParam);
	

		while (true)
		{		
			if (pApp->m_write_queue.empty()==true)
			{
				if (pApp->m_bthreadRunning == false)
				{
					break;
				}
				::ResetEvent(pApp->m_hEvent);
				::WaitForSingleObject(pApp->m_hEvent, INFINITE);
			}

			write_data *p = 0;
			::EnterCriticalSection(&pApp->m_criQueueLock);
			p = pApp->m_write_queue.front();
			pApp->m_write_queue.pop();
			::LeaveCriticalSection(&pApp->m_criQueueLock);
			p->WriteData(p->data, p->nlen);
			delete p;
			p = 0;
			::Sleep(10);
			ret = 1;
		}

	return ret;
}


communication_write_data::communication_write_data(CLocalCommunication *pcommunicater)
	: m_pcommunicater(pcommunicater)
{

}

int communication_write_data::WriteData(unsigned char *data, int len)
{
	int nret = -1;
	if(m_pcommunicater!=0 && m_pcommunicater->ConnectedNamedPipe())
	{
		CNotifyTextCommunicateData notify_data;
		notify_data.ComposeData(data, len);
		nret = m_pcommunicater->WriteCommunicationData(0, notify_data);
		m_pcommunicater->DisconnectedNamePipe();
	}
	return nret;
}

file_write_data::file_write_data(MyString str_log)
	: m_str_log(str_log)
{

}

int file_write_data::WriteData(unsigned char *data, int len)
{
	int nret = -1;

	CLogFile log_file;
	
	nret = log_file.WriteLog(m_str_log);
	
	return nret;
}
