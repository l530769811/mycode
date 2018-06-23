#ifndef _COMMUNICATEDATA_H__
#define _COMMUNICATEDATA_H__
#include "loaca_ui_communicate_define.h"
#include "my_assist_define.h"

class CLocalCommunication;
class _LOCALUICOMMUNICATEDEFINE_IMPORT_EXPORT_DLL_ CCommunicateData
{
	friend CLocalCommunication;
public:
	
	CCommunicateData(void);
	virtual ~CCommunicateData(void);

public:	
	virtual bool IsType(const unsigned char *data, const int &data_len) = 0;
	virtual int ComposeData(const unsigned char *data, const int &data_len) = 0;	
	virtual int DecomposeData(unsigned char *data, int &data_len) const = 0 ;
	virtual MyString toString() const = 0;

};

#endif