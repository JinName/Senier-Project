#pragma once

#include "stdafx.h"
#include "ClientSession.h"

class ClientSession;

/*
클라이언트 관리자.
클라이언트 증감이나 생성, 파괴등을 맡음.
클라이언트 관리를 위해 stl map 자료구조를 이용.
접근 제한을 위해 CRITICAL_SECTION을 사용함.

----- METHOD -----
CreateClientSession() : ClientSession 클래스를 이용해 accept 시에 클라이언트의 정보를 확인 저장하며, 
						이를 SessionManager 내의 자료구조에 소켓과 매칭하여 저장.

DeleteClientSession() : 클라이언트가 접속 종료 시에 자료구조에서 해당 ClientSession 에 맵핑되어있는
						소켓 정보를 지운뒤에 ClientSession 에 할당되어있는 메모리를 delete 해준다.

IncreaseClientCount() : 클라이언트가 accept 에 성공하면, SessionManager 내에 클라이언트 갯수를 증가.
DecreaseClientCount() : 클라이언트가 접속 종료 시에 클라이언트 갯수 감소.
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
	ClientList m_ClientList;	// 클라이언트 리스트

	int m_ClientCount;		// 클라이언트의 갯수

	CRITICAL_SECTION m_CS;	// 공유자원 접근제한을 위한 CRITICAL_SECTION
};

//전역으로 사용하기 위해 extern 변수로 정의.
extern SessionManager* g_pSessionManager;