#include "LocalCommunication.h"
#include <tchar.h>
#include "CommunicateData.h"

CLocalCommunication::CLocalCommunication(MyString strName, int nenum)
	: m_enum(nenum)
	, m_strBaseName(strName)
	, m_hPipe(INVALID_HANDLE_VALUE)
	, m_hPipeFile(INVALID_HANDLE_VALUE)
	, m_bConnect(FALSE)
	, m_bClose(FALSE)
{

	size_t pos = MyString::npos;
	while ( (pos=m_strBaseName.find(_T("\\"))) != MyString::npos)
	{
		m_strBaseName.replace(pos, 1, _T(""));
	}

	while ( (pos=m_strBaseName.find(_T("/"))) != MyString::npos)
	{
		m_strBaseName.replace(pos, 1, _T(""));
	}
	this->CreateNamePipe();
}


CLocalCommunication::~CLocalCommunication(void)
{
	m_bClose = TRUE;
	if(m_hPipe!=INVALID_HANDLE_VALUE && m_hPipe!=0)
	{
		::CloseHandle(m_hPipe);
		m_hPipe = INVALID_HANDLE_VALUE;
	}

	if(m_hPipeFile!=INVALID_HANDLE_VALUE && m_hPipeFile!=0)
	{
		::CloseHandle(m_hPipeFile);
		m_hPipeFile = INVALID_HANDLE_VALUE;
	}
	
}

BOOL CLocalCommunication::CreateNamePipe()
{
	BOOL bret = FALSE;
	m_strName = m_strBaseName;
	MyString strBaseName(_T("\\\\.\\pipe\\"));
	m_strName = strBaseName + m_strBaseName + _T("_channel");

	if (m_enum==0)
	{
		m_hPipe = ::CreateNamedPipe(m_strName.c_str(), 
			PIPE_ACCESS_DUPLEX,
			PIPE_READMODE_MESSAGE | PIPE_TYPE_MESSAGE | PIPE_WAIT, 
			1, 1024, 1024, 5000,
			0);
		bret = (m_hPipe==INVALID_HANDLE_VALUE || m_hPipe==0) ? FALSE : TRUE;
	}
	else
	{
		bret = TRUE;
	}
	

	return bret;
}

BOOL CLocalCommunication::ConnectedNamedPipe()
{
	BOOL resulte = FALSE;
	if (m_hPipe!=INVALID_HANDLE_VALUE && m_hPipe!=0)
	{
		m_bConnect = ::ConnectNamedPipe(m_hPipe, 0);
		
	}
	else
	{
		if( 
			(m_hPipeFile = CreateFile(m_strName.c_str(),
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE, 
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0,              // default attributes 
			NULL)
			) ==  INVALID_HANDLE_VALUE)
		{
			if(::WaitNamedPipe(m_strName.c_str(), 100)==FALSE)
			{
				DWORD err_code = ::GetLastError();
				if (err_code!=ERROR_SEM_TIMEOUT && err_code!=ERROR_SUCCESS && err_code!=ERROR_FILE_NOT_FOUND)
				{
					;
				}

				if (err_code==ERROR_FILE_NOT_FOUND)
				{
					::Sleep(100);
				}
			}
		}

		m_bConnect = m_hPipeFile==INVALID_HANDLE_VALUE ? FALSE : TRUE;
	}
	

	return m_bConnect;
}

BOOL CLocalCommunication::DisconnectedNamePipe()
{
	BOOL bDisconnect = FALSE;
	if(m_bConnect == TRUE)
	{
		if (m_hPipeFile!=INVALID_HANDLE_VALUE && m_hPipeFile !=0)
		{
			bDisconnect = DisconnectNamedPipe(m_hPipeFile);
			if (bDisconnect==FALSE)
			{
				DWORD error = ::GetLastError();
			}
		}
	}
	return bDisconnect;
}

int CLocalCommunication::WriteCommunicationData(const unsigned short &key, const CCommunicateData &data)
{
	int nret = -1;
	unsigned char input_data[2048] = {0};
	int data_len = 2047;

	HANDLE hTmp = INVALID_HANDLE_VALUE;
	hTmp = m_hPipeFile;

	if(hTmp != INVALID_HANDLE_VALUE && hTmp!=0){

		data_len = data.DecomposeData(input_data, data_len);
		unsigned char decompose_data[2032] = {0};
		int len = 2031;
		len = data.DecomposeData(decompose_data, len);

		memcpy(input_data, &key, 16);
		memcpy(input_data+16, &len, 32);
		memcpy(input_data+16+32, decompose_data, len);
		data_len = len + 16 + 32;

		unsigned long nwrite_len = 0;
		if (::WriteFile(hTmp, input_data, data_len, &nwrite_len, 0)!=FALSE)
		{
			nret = nwrite_len;
		}
		else
		{
			if (m_bClose!=TRUE)
			{
				::CloseHandle(hTmp);				
				if(CreateNamePipe()==TRUE)
				{
					ConnectedNamedPipe();
				}
			}
		}
	}

	return nret;
}

int CLocalCommunication::ReadCommunicationData(unsigned short &key, CCommunicateData &data)
{
	int nret = -1;
	unsigned char output_data[2048] = {0};
	int data_len = 2047;

	HANDLE hTmp = INVALID_HANDLE_VALUE;
	
	hTmp = m_hPipe;	

	if(hTmp != INVALID_HANDLE_VALUE && hTmp!=0)
	{
		unsigned long nReadLen = 0;
		if(::ReadFile(hTmp, output_data, data_len, &nReadLen, 0)!=FALSE)
		{
			unsigned short key_short = (unsigned short)output_data;
			data_len = (int)output_data+16;

			memcpy(&key_short, output_data, 16);
			memcpy(&data_len, output_data+16, 32);
			unsigned char *poutput = output_data+16+32;
			if(data.IsType(output_data+16+32, data_len) ==true){
				data_len = data.ComposeData(output_data+16+32, data_len);
				nret = data_len;
			}
			else
			{
				nret = -2;
			}
		}
		else
		{
			if (m_bClose!=TRUE)
			{
				::CloseHandle(hTmp);				
				if(CreateNamePipe()==TRUE)
				{
					ConnectedNamedPipe();
				}
			}
		}
	}

	return nret;
}
