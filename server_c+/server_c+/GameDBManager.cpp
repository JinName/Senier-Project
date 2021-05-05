#include "GameDBManager.h"

GameDBManager* g_pGameDBManager = nullptr;

GameDBManager::GameDBManager()
{
}

bool GameDBManager::Login(ClientSession* client, char* id, char* password)
{
	char query[MAX_QUERY_LEN];
	char isLogin[2];

	memset(query, 0, MAX_QUERY_LEN);
	memset(isLogin, 0, 2);

	g_pCrypt->Decrypt(password);

	sprintf_s(query, "call search_user('%s', '%s');", id, password);

	// ID와 PW 가 일치하는 정보가 DB에 존재하는지 확인
	MYSQL_RES* resLogin = GetSQLResult(query);

	if (resLogin == NULL)
	{
		cout << "GameDBManager > Login() > LOGIN INFO res IS NULL" << endl;

		NextSQLResult();
		FreeSQLResult(resLogin);

		return false;
	}

	MYSQL_ROW row = mysql_fetch_row(resLogin);
	// 일치하는 정보가 없으면 RETURN FALSE;
	if (row == NULL)
	{
		cout << "GameDBManager > Login() > LOGIN INFO row IS NULL" << endl;

		NextSQLResult();
		FreeSQLResult(resLogin);

		return false;
	}	

	// 필드 2번째 값인 is_login 이 true인지 확인
	if (strcmp(row[2], "1") == 0)
	{
		cout << "GameDBManager > Login() > ALREADY LOGIN.." << endl;

		NextSQLResult();
		FreeSQLResult(resLogin);

		return false;
	}

	NextSQLResult();
	FreeSQLResult(resLogin);
	memset(query, 0, MAX_QUERY_LEN);

	// 정보 확인후 DB에서 로그인상태로 변경
	sprintf_s(query, "call update_user_islogin('%s', '%d');", id, 1);

	client->SetID(id);
	client->SetIsLogin(true);

	// 쿼리문 수행과 그 결과값
	MYSQL_RES* res_update = GetSQLResult(query);

	NextSQLResult();
	FreeSQLResult(res_update);

	cout << "GameDBManager > Login() > LOGIN SUCCESS" << endl;

	return true;
}

bool GameDBManager::Logout(ClientSession* client)
{
	char query[MAX_QUERY_LEN];

	sprintf_s(query, "call update_user_islogin('%s', '%d');", client->GetID(), 0 );

	MYSQL_RES* res = GetSQLResult(query);

	NextSQLResult();
	FreeSQLResult(res);

	client->CleanID();
	client->SetIsLogin(false);

	cout << "GameDBManager > Logout() > LOGOUT SUCCESS" << endl;

	return true;
}

bool GameDBManager::IncreaseWin(ClientSession* client)
{
	char query[MAX_QUERY_LEN];

	sprintf_s(query, "call increase_win('%s');", client->GetID());

	MYSQL_RES* res = GetSQLResult(query);

	NextSQLResult();
	FreeSQLResult(res);

	cout << "GameDBManager > Logout() > LOGOUT SUCCESS" << endl;

	return true;
}

bool GameDBManager::IncreaseLose(ClientSession* client)
{
	char query[MAX_QUERY_LEN];

	sprintf_s(query, "call increase_lose('%s');", client->GetID());

	MYSQL_RES* res = GetSQLResult(query);

	NextSQLResult();
	FreeSQLResult(res);

	cout << "GameDBManager > Logout() > LOGOUT SUCCESS" << endl;

	return true;
}