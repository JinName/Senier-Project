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
함수명 : CreateClientSession()
인자값 : SOCKET socket
기능 : clientsession을 만들고 관리 객체에 등록함.
*/
ClientSession* SessionManager::CreateClientSession(SOCKET socket)
{
	ClientSession* client = new ClientSession(socket);

	cout << "create ClientSession.." << endl;
	m_ClientList.insert(ClientList::value_type(socket, client));
	return client;
}

/*
함수명 : DeleteClientSession()
인자값 : ClientSession * Client
기능 : 접속이 끊기는 clientsession을 삭제함.
*/
void SessionManager::DeleteClientSession(ClientSession* client)
{
	m_ClientList.erase(client->GetSocket());

	delete client;

	cout << "client delete" << endl;
}

/*
함수명 : IncreaseClientCount()
인자값 :
기능 : 접속된 clientsession이 생기면 전체 클라이언트 갯수를 증가시킴.
	   서로 다른 객체들끼리 동기화시키기 위해 CRITICAL_SECTION을 사용.
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
함수명 : DecreaseClientCount()
인자값 :
기능 : 접속이 끊기는 clientsession이 생기면 전체 클라이언트 갯수를 감소시킴.
	   서로 다른 객체들끼리 동기화시키기 위해 CRITICAL_SECTION을 사용.
*/
int SessionManager::DecreaseClientCount()
{
	EnterCriticalSection(&m_CS);
	m_ClientCount--;
	cout << "client count decrease.." << endl;
	LeaveCriticalSection(&m_CS);

	return m_ClientCount;
}