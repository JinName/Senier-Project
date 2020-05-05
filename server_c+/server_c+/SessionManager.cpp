#include "SessionManager.h"

SessionManager* g_pSessionManager = nullptr;

SessionManager::SessionManager() : mClientCount(0)
{
	InitializeCriticalSection(&CS);
}

SessionManager::~SessionManager()
{
}

/*
�Լ��� : CreateClientSession()
���ڰ� : SOCKET sock
��� : clientsession�� ����� ���� ��ü�� �����.
*/
ClientSession* SessionManager::CreateClientSession(SOCKET sock)
{
	ClientSession* client = new ClientSession(sock);

	cout << "create ClientSession.." << endl;
	mClientList.insert(ClientList::value_type(sock, client));
	return client;
}

/*
�Լ��� : DeleteClientSession()
���ڰ� : ClientSession * Client
��� : ������ ����� clientsession�� ������.
*/
void SessionManager::DeleteClientSession(ClientSession* client)
{
	mClientList.erase(client->GetSocket());

	delete client;

	cout << "client delete" << endl;
}

/*
�Լ��� : IncreaseClientCount()
���ڰ� :
��� : ���ӵ� clientsession�� ����� ��ü Ŭ���̾�Ʈ ������ ������Ŵ.
	   ���� �ٸ� ��ü�鳢�� ����ȭ��Ű�� ���� CRITICAL_SECTION�� ���.
*/
int SessionManager::IncreaseClientCount()
{
	EnterCriticalSection(&CS);
	mClientCount++;
	cout << "client count increase.." << endl;
	LeaveCriticalSection(&CS);

	return mClientCount;
}

/*
�Լ��� : DecreaseClientCount()
���ڰ� :
��� : ������ ����� clientsession�� ����� ��ü Ŭ���̾�Ʈ ������ ���ҽ�Ŵ.
	   ���� �ٸ� ��ü�鳢�� ����ȭ��Ű�� ���� CRITICAL_SECTION�� ���.
*/
int SessionManager::DecreaseClientCount()
{
	EnterCriticalSection(&CS);
	mClientCount--;
	cout << "client count decrease.." << endl;
	LeaveCriticalSection(&CS);

	return mClientCount;
}