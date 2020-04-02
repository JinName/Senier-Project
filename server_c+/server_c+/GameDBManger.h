#pragma once
#include "MYSQLManager.h"

// GameDBManger
// ���ӿ��� DB ������ ���� ����ϴ� Ŭ����
// MYSQLManager �� ��ӹ޾� �� ���ӿ� �ʿ��� DB ���ٹ���� ������ ������

class GameDBManger : public MYSQLManager
{
public:
	GameDBManger() {}
	~GameDBManger() {}

	bool	Login(const char* _id, const char* _pw);
};

//�������� ����ϱ� ���� extern ������ ����.
extern GameDBManger* g_pGameDBManager;