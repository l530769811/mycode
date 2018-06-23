#ifndef _NOTIFYTEXTCOMMUNICATEDATA_H__
#define _NOTIFYTEXTCOMMUNICATEDATA_H__
#include "CommunicateData.h"
#include "my_assist_define.h"
#include <tchar.h>

class CNotifyTextCommunicateData :
	public CCommunicateData
{
public:
	CNotifyTextCommunicateData(MyString strNotifyText);
	CNotifyTextCommunicateData(void);
	virtual ~CNotifyTextCommunicateData(void);

public:
	virtual bool IsType(const unsigned char *data, const int &data_len);
	virtual int ComposeData(const unsigned char *data, const int &data_len);	
	virtual int DecomposeData(unsigned char *data, int &data_len) const;
	virtual MyString toString() const;
private:
	MyString m_strNotifyText;
	MyString m_strType;
	static  const MyString m_strRequest;

};

#endif //_NOTIFYTEXTCOMMUNICATEDATA_H__