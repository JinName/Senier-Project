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
			// make packet
			SHEAD head;
			head.mCmd = (unsigned char)PROTOCOL::MATCH_RQ;
			head.mDataSize = sizeof(SHEAD) + sizeof(SMATCH);

			SMATCH match;
			match.mInMatch = true;

			char buffer[MAX_BUFSIZE];
			int bufferSize = head.mDataSize;
			memset(buffer, 0, MAX_BUFSIZE);
			memcpy(buffer, (char*)&head, sizeof(SHEAD));
			memcpy(buffer + sizeof(SHEAD), (char*)&match, sizeof(SMATCH));

			// send packet
			Network::GetInstance()->SendPacket(buffer, bufferSize);

			m_bGameStart = true;
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