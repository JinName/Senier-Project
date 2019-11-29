#include "Title.h"
#include "Network.h"
#include "PacketList.h"


CTitle::CTitle()
{
}

CTitle::~CTitle()
{
}

void CTitle::OnInit(LPDIRECT3DDEVICE9 _pDevice)
{
	m_iGameScene_Num = TITLE;
	Button_GameStart.Init(_pDevice, L"2D_Sprites\\GameStart.png", 340.0f, 280.0f, 300.0f, 70.0f);
	Button_Exit.Init(_pDevice, L"2D_Sprites\\Exit.png", 340.0f, 380.0f, 264.0f, 72.0f);
}

void CTitle::OnUpdate(LPDIRECT3DDEVICE9 _pDevice)
{
	if (CInput::Get_Instance()->IsMouseKeyPressed(DIMOUSE_XAXISAB))
	{
		long x, y, z;
		CInput::Get_Instance()->GetMouseRelativePosition(x, y, z);
		if (Button_GameStart.On_Click(x, y))
		{			
			// 보내려는 구조체 생성, 값 설정
			SMATCH match;
			match.mInMatch = true;		

			// 서버에 패킷전송
			Network::GetInstance()->SendPacket(PROTOCOL::MATCH_RQ, (char*)&match, sizeof(SMATCH));

			//m_bGameStart = true;
		}

		if (Button_Exit.On_Click(x, y))
		{
			m_bExit = true;
		}
	}
}

void CTitle::OnRender(LPDIRECT3DDEVICE9 _pDevice)
{
	Button_GameStart.Render();
	Button_Exit.Render();
}

void CTitle::OnCleanup(LPDIRECT3DDEVICE9 _pDevice)
{
	Button_GameStart.Clean();
	Button_Exit.Clean();
}