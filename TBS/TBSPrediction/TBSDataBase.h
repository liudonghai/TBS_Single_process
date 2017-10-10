#pragma once
#include <string>
#include "stdafx.h"
#include "sqlite3.h"
using namespace std;

// CTBSDataBase ÃüÁîÄ¿±ê

class CTBSDataBase : public CObject
{
public:
	CTBSDataBase(string strDBName);
	virtual ~CTBSDataBase();
	
public:
	
	string			strDataPath;
	sqlite3			*m_pDataBase;
	static CMutex	*m_pMutex;

	INT tbs_db_init();
	INT tbs_db_sql_exec(string strExpression);
	INT tbs_db_data_query(string strExpression, CHAR ***pData, INT* iRow, INT* iColum);
};


