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
함수명 : CreateClientSession()
인자값 : SOCKET sock
기능 : clientsession을 만들고 관리 객체에 등록함.
*/
ClientSession* SessionManager::CreateClientSession(SOCKET sock)
{
	ClientSession* client = new ClientSession(sock);

	cout << "create ClientSession.." << endl;
	mClientList.insert(ClientList::value_type(sock, client));
	return client;
}

/*
함수명 : DeleteClientSession()
인자값 : ClientSession * Client
기능 : 접속이 끊기는 clientsession을 삭제함.
*/
void SessionManager::DeleteClientSession(ClientSession* client)
{
	mClientList.erase(client->GetSocket());

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
	EnterCriticalSection(&CS);
	mClientCount++;
	cout << "client count increase.." << endl;
	LeaveCriticalSection(&CS);

	return mClientCount;
}

/*
함수명 : DecreaseClientCount()
인자값 :
기능 : 접속이 끊기는 clientsession이 생기면 전체 클라이언트 갯수를 감소시킴.
	   서로 다른 객체들끼리 동기화시키기 위해 CRITICAL_SECTION을 사용.
*/
int SessionManager::DecreaseClientCount()
{
	EnterCriticalSection(&CS);
	mClientCount--;
	cout << "client count decrease.." << endl;
	LeaveCriticalSection(&CS);

	return mClientCount;
}