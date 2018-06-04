#ifndef _CLIENT_H__
#define _CLIENT_H__

class COperater;
class CClient
{
public:
	CClient(void);
	virtual ~CClient(void);

	virtual void login_in();
	virtual void login_out();
	virtual bool do_operate(COperater &op) = 0;
};

#endif //_CLIENT_H__