#ifndef _VIPCARDMANAGER_H__
#define _VIPCARDMANAGER_H__

class CDBSqlManager;
class CDBDoOperator;
class CVIPCardManager
{
public:
	CVIPCardManager(CDBSqlManager *pdbmgr);
	virtual ~CVIPCardManager(void);

	int DoOperate(CDBDoOperator &op);

private:
	CDBSqlManager *m_pdbmgr;
};

#endif _VIPCARDMANAGER_H__