#include "SessionManager.h"

SessionManager* g_pSessionManager = nullptr;

SessionManager::SessionManager() : m_ClientCount(0)
{
	InitializeCriticalSection(&m_CS);
}

SessionManager::~SessionManager()
{
}

/*
�Լ��� : CreateClientSession()
���ڰ� : SOCKET socket
��� : clientsession�� ����� ���� ��ü�� �����.
*/
ClientSession* SessionManager::CreateClientSession(SOCKET socket)
{
	ClientSession* client = new ClientSession(socket);

	cout << "create ClientSession.." << endl;
	m_ClientList.insert(ClientList::value_type(socket, client));
	return client;
}

/*
�Լ��� : DeleteClientSession()
���ڰ� : ClientSession * Client
��� : ������ ����� clientsession�� ������.
*/
void SessionManager::DeleteClientSession(ClientSession* client)
{
	m_ClientList.erase(client->GetSocket());

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
	EnterCriticalSection(&m_CS);
	m_ClientCount++;
	cout << "client count increase.." << endl;
	LeaveCriticalSection(&m_CS);

	return m_ClientCount;
}

/*
�Լ��� : DecreaseClientCount()
���ڰ� :
��� : ������ ����� clientsession�� ����� ��ü Ŭ���̾�Ʈ ������ ���ҽ�Ŵ.
	   ���� �ٸ� ��ü�鳢�� ����ȭ��Ű�� ���� CRITICAL_SECTION�� ���.
*/
int SessionManager::DecreaseClientCount()
{
	EnterCriticalSection(&m_CS);
	m_ClientCount--;
	cout << "client count decrease.." << endl;
	LeaveCriticalSection(&m_CS);

	return m_ClientCount;
}