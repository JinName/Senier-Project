#include "GameDBManager.h"

GameDBManager* g_pGameDBManager = nullptr;

GameDBManager::GameDBManager()
{
}

bool GameDBManager::Login(ClientSession* _client, char* _id, char* _pw)
{
	char query[MAX_QUERY_LEN];
	char isLogin[2];

	memset(query, 0, MAX_QUERY_LEN);
	memset(isLogin, 0, 2);

	g_pCrypt->Decrypt(_pw);

	sprintf_s(query, "select * from user_info where id = '%s' and pw = '%s'", _id, _pw);

	// ID와 PW 가 일치하는 정보가 DB에 존재하는지 확인
	MYSQL_RES* res = GetSQLResult(query);

	MYSQL_ROW row = mysql_fetch_row(res);
	// 일치하는 정보가 없으면 RETURN FALSE;
	if (row == NULL)
	{
		cout << "GameDBManager > Login() > LOGIN INFO RESULT IS NULL" << endl;

		return false;
	}	

	// 필드 2번째 값인 is_login 이 true인지 확인
	if (strcmp(row[2], "1") == 0)
	{
		cout << "GameDBManager > Login() > ALREADY LOGIN.." << endl;

		return false;
	}

	FreeSQLResult(res);
	memset(query, 0, MAX_QUERY_LEN);

	// 정보 확인후 DB에서 로그인상태로 변경
	sprintf_s(query, "update user_info set is_login = '%d'  where id = '%s'", 1, _id);

	_client->SetID(_id);
	_client->SetIsLogin(true);

	// 쿼리문 수행과 그 결과값
	res = GetSQLResult(query);

	FreeSQLResult(res);

	cout << "GameDBManager > Login() > LOGIN SUCCESS" << endl;

	return true;
}

bool GameDBManager::Logout(ClientSession* _client)
{
	char query[MAX_QUERY_LEN];

	sprintf_s(query, "update user_info set is_login = '%d'  where id = '%s'", 0, _client->GetID());

	MYSQL_RES* res = GetSQLResult(query);

	FreeSQLResult(res);

	_client->CleanID();
	_client->SetIsLogin(false);

	cout << "GameDBManager > Logout() > LOGOUT SUCCESS" << endl;

	return true;
}