#include "GameDBManger.h"

GameDBManger* g_pGameDBManager = nullptr;

bool GameDBManger::Login(const char* _id, const char* _pw)
{
	char query[MAX_QUERY_LEN];

	sprintf_s(query, "select * from user_info where id = '%s' and pw = '%s'", _id, _pw);

	// ID�� PW �� ��ġ�ϴ� ������ DB�� �����ϴ��� Ȯ��
	MYSQL_RES* res = GetSQLResult(query);

	// ��ġ�ϴ� ������ ������ RETURN FALSE;
	if (mysql_fetch_row(res) == NULL)
	{
		cout << "GameDBManager > Login() > LOGIN INFO RESULT IS NULL" << endl;

		return false;
	}

	FreeSQLResult(res);

	cout << "GameDBManager > Login() > LOGIN SUCCESS" << endl;

	return true;
}