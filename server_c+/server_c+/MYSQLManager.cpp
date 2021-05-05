#include "MYSQLManager.h"

MYSQLManager::MYSQLManager()
	:m_pConnection(NULL)
{
}

MYSQLManager::~MYSQLManager()
{
}

bool MYSQLManager::Init()
{
	mysql_init(&m_ConnectInfo);

	return true;
}

bool MYSQLManager::Connect()
{
	m_pConnection = mysql_real_connect(&m_ConnectInfo, DB_HOST, DB_USER, DB_PW, DB_NAME, 3306, (char*)NULL, 0);

	if (m_pConnection == NULL)
	{
		// except error
		fprintf(stderr, "mysql connection error : %s\n", mysql_error(&m_ConnectInfo));

		return false;
	}

	return true;
}

bool MYSQLManager::IsConnected()
{
	if (m_pConnection == NULL)
		return false;

	return true;
}

bool MYSQLManager::Disconnect()
{
	// close mysql connection
	mysql_close(m_pConnection);

	return true;
}

MYSQL_RES* MYSQLManager::GetSQLResult(const char* sqlQuery)
{
	// mysql_query() : success -> return 0;
	int queryStat = mysql_query(m_pConnection, sqlQuery);

	if (queryStat != 0)
	{
		fprintf(stderr, "mysql query error : %s\n", mysql_error(&m_ConnectInfo));

		return NULL;
	}

	return mysql_store_result(m_pConnection);
}

bool MYSQLManager::FreeSQLResult(MYSQL_RES* res)
{
	mysql_free_result(res);

	return true;
}

void MYSQLManager::NextSQLResult()
{
	while (true)
	{
		if (mysql_next_result(m_pConnection) == -1)
			break;
	}
}