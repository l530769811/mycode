#ifndef _NETSOCKETPROXY_H__
#define _NETSOCKETPROXY_H__


class CNetSocketProxy
{
public:
	CNetSocketProxy(void);
	virtual  ~CNetSocketProxy(void);
	
	virtual int SendData(const unsigned char* data, long len) = 0;
};


#endif //_NETSOCKETPROXY_H__