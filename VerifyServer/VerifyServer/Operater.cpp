#include "Operater.h"

COperater::COperater(void)
{

}

COperater::~COperater(void)
{

}

bool COperater::_OperatePrepare()
{
	return true;
}

bool COperater::_DoOperate()
{
	return false;
}

bool COperater::_OperateDone()
{
	return false;
}

void COperater::Operate()
{
	if (!_OperatePrepare())
	{
		return;
	}

	if(!_DoOperate())
	{
		return;
	}

	_OperateDone();
}