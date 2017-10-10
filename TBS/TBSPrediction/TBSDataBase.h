#pragma once
#include "stdafx.h"
#include <string>
#include "sqlite3.h"
using namespace std;
// CTBSDataBase ÃüÁîÄ¿±ê

class CTBSDataBase : public CObject
{
public:
	CTBSDataBase();
	CTBSDataBase(string strDBName);
	virtual ~CTBSDataBase();
	
	static CMutex *pMetux;
	string		strDataPath;
	sqlite3		*m_pDataBase;

	INT tbs_db_init();
	INT tbs_db_sql_exec(string strExpression);
	INT tbs_db_data_query(string strExpression,CHAR ***pData,INT* iRow,INT* iColum);
};


