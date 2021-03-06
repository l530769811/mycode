#ifndef __ESOP_COPERATE_H__
#define __ESOP_COPERATE_H__

class COperater
{
public:
	COperater(void);
	virtual ~COperater(void);

public:
	void Operate(void);
	
protected:
	virtual bool _OperatePrepare(void);
	virtual bool _DoOperate(void) = 0;
	virtual bool _OperateDone(void);
};

#endif //__ESOP_COPERATE_H__