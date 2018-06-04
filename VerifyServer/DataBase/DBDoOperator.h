#ifndef _DBDOOPERATOR_H__
#define _DBDOOPERATOR_H__
#include "database_define.h"

class CDBSqlManager;
class _DATABASE_IMPORT_EXPORT_DLL_ CDBDoOperator
{
public:
	CDBDoOperator(void);
	virtual ~CDBDoOperator(void);

public:
	virtual int Exec(CDBSqlManager *pdb) = 0;
};

#endif //_DBDOOPERATOR_H__