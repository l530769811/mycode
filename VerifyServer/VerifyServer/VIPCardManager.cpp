#include "VIPCardManager.h"
#include "DBDoOperator.h"

CVIPCardManager::CVIPCardManager(CDBSqlManager *pdbmgr)
	: m_pdbmgr(pdbmgr)
{
}


CVIPCardManager::~CVIPCardManager(void)
{
}

int CVIPCardManager::DoOperate(CDBDoOperator &op)
{
	int nret = -1;
	nret = op.Exec(m_pdbmgr);
	return nret;
}