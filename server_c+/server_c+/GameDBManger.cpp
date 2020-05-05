#include "GameDBManger.h"

GameDBManger* g_pGameDBManager = nullptr;

bool GameDBManger::Login(const char* _id, const char* _pw)
{
	char query[MAX_QUERY_LEN];

	sprintf_s(query, "select * from user_info where id = '%s' and pw = '%s'", _id, _pw);

	// ID와 PW 가 일치하는 정보가 DB에 존재하는지 확인
	MYSQL_RES* res = GetSQLResult(query);

	// 일치하는 정보가 없으면 RETURN FALSE;
	if (mysql_fetch_row(res) == NULL)
	{
		cout << "GameDBManager > Login() > LOGIN INFO RESULT IS NULL" << endl;

		return false;
	}

	FreeSQLResult(res);

	cout << "GameDBManager > Login() > LOGIN SUCCESS" << endl;

	return true;
}