#ifndef _LOCALCOMMUNICATION_H__
#define _LOCALCOMMUNICATION_H__
#include "loaca_ui_communicate_define.h"
#include "my_assist_define.h"

#define BUFSIZE 512

class CCommunicateData;
class _LOCALUICOMMUNICATEDEFINE_IMPORT_EXPORT_DLL_ CLocalCommunication
{
public:
	CLocalCommunication(MyString strName, int nenum = 0);
	~CLocalCommunication(void);

public:
	BOOL CreateNamePipe();
	BOOL ConnectedNamedPipe();
	BOOL DisconnectedNamePipe();
	int ReadCommunicationData(unsigned short &key, CCommunicateData &data);
	int WriteCommunicationData(const unsigned short &key, const CCommunicateData &data);

private:
	int m_enum;
	MyString m_strBaseName;
	MyString m_strName;
	HANDLE	m_hPipe;
	HANDLE	m_hPipeFile;
	BOOL m_bConnect;
	BOOL m_bClose;
};

#endif _LOCALCOMMUNICATION_H__