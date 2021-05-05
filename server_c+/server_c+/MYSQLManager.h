#pragma once
#include <iostream>
using namespace std;

#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

#define DB_HOST			"127.0.0.1"
#define DB_USER			"root"
#define DB_PW			"tmddnjs2@1324"
#define DB_NAME			"login_test"

#define MAX_QUERY_LEN	256

// MYSQLManager
// 기본적인 mysql 과의 연결을 담당하는 클래스
// 이를 상속받아 게임에 맞는 GameDBManager 를 만드는 것이 목적

class MYSQLManager
{
public:
	MYSQLManager();
	~MYSQLManager();

	bool		Init();

	bool		Connect();
	bool		IsConnected();
	bool		Disconnect();

	MYSQL_RES*	GetSQLResult(const char* sqlQuery);
	bool		FreeSQLResult(MYSQL_RES* res);

	void		NextSQLResult();

protected:
	MYSQL*		m_pConnection;		// MYSQL 함수 핸들용 포인터
	MYSQL		m_ConnectInfo;		// MYSQL 정보를 담는 구조체
};