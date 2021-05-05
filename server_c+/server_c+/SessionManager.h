#pragma once

#include "stdafx.h"
#include "ClientSession.h"

class ClientSession;

/*
Ŭ���̾�Ʈ ������.
Ŭ���̾�Ʈ �����̳� ����, �ı����� ����.
Ŭ���̾�Ʈ ������ ���� stl map �ڷᱸ���� �̿�.
���� ������ ���� CRITICAL_SECTION�� �����.

----- METHOD -----
CreateClientSession() : ClientSession Ŭ������ �̿��� accept �ÿ� Ŭ���̾�Ʈ�� ������ Ȯ�� �����ϸ�, 
						�̸� SessionManager ���� �ڷᱸ���� ���ϰ� ��Ī�Ͽ� ����.

DeleteClientSession() : Ŭ���̾�Ʈ�� ���� ���� �ÿ� �ڷᱸ������ �ش� ClientSession �� ���εǾ��ִ�
						���� ������ ����ڿ� ClientSession �� �Ҵ�Ǿ��ִ� �޸𸮸� delete ���ش�.

IncreaseClientCount() : Ŭ���̾�Ʈ�� accept �� �����ϸ�, SessionManager ���� Ŭ���̾�Ʈ ������ ����.
DecreaseClientCount() : Ŭ���̾�Ʈ�� ���� ���� �ÿ� Ŭ���̾�Ʈ ���� ����.
*/
class SessionManager
{
public:
	SessionManager();
	~SessionManager();

	ClientSession* CreateClientSession(SOCKET socket);

	void DeleteClientSession(ClientSession* client);

	int IncreaseClientCount();
	int DecreaseClientCount();

private:
	typedef map<SOCKET, ClientSession*> ClientList;	// socket - clientsession mapping
	ClientList m_ClientList;	// Ŭ���̾�Ʈ ����Ʈ

	int m_ClientCount;		// Ŭ���̾�Ʈ�� ����

	CRITICAL_SECTION m_CS;	// �����ڿ� ���������� ���� CRITICAL_SECTION
};

//�������� ����ϱ� ���� extern ������ ����.
extern SessionManager* g_pSessionManager;