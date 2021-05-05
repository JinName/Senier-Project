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
// �⺻���� mysql ���� ������ ����ϴ� Ŭ����
// �̸� ��ӹ޾� ���ӿ� �´� GameDBManager �� ����� ���� ����

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
	MYSQL*		m_pConnection;		// MYSQL �Լ� �ڵ�� ������
	MYSQL		m_ConnectInfo;		// MYSQL ������ ��� ����ü
};