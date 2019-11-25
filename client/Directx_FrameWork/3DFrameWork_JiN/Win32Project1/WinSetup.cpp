#include "WinSetup.h"

CWinSetup::CWinSetup()
	:m_hWnd(NULL),
	m_hInst(NULL),
	m_game(NULL)
{
	// 윈도우 크기 설정
	m_rc = { 0, 0, 640, 480 };
}

CWinSetup::CWinSetup(HINSTANCE hInstance)
	:m_hWnd(NULL),
	m_hInst(hInstance),
	m_game(NULL)
{
	// 윈도우 크기 설정
	m_rc = { 0, 0, 960, 720 };	
}

// 변수값의 변화를 반영하기 위한 MsgProc - 일반 멤버변수에 접근 가능
LRESULT WINAPI CWinSetup::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		m_game->Cleanup();
		delete m_game;
		m_game = NULL;
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}


// 윈도우 생성시 값을 넘겨주기 위해 정적변수로 선언된 WndProc - 정적함수이므로 일반 멤버변수에 접근 불가
LRESULT WINAPI CWinSetup::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CWinSetup *pWin = (CWinSetup*)GetWindowLong(hWnd, GWL_USERDATA);

	if (!pWin)
		return DefWindowProc(hWnd, msg, wParam, lParam);

	return pWin->MsgProc(hWnd, msg, wParam, lParam);
}



bool CWinSetup::initWindow()
{
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);

	m_wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"3DFRAMEWORK_ver1.0", NULL
	};
	
	if (!RegisterClassEx(&m_wc))
		return FALSE;
	else
		return TRUE;
}

void CWinSetup::createWindow()
{
	//윈도우 생성
	AdjustWindowRect(&m_rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindow(L"3DFRAMEWORK_ver1.0", L"3DFRAMEWORK", WS_OVERLAPPEDWINDOW,
		350, 120, m_rc.right - m_rc.left, m_rc.bottom - m_rc.top, NULL, NULL, m_hInst,
		NULL);

	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);	

	// 윈도우 출력
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hWnd);
}

VOID CWinSetup::Init()
{
	// 키보드 생성
	CInput::Get_Instance()->InitDirectInput(m_hInst, m_hWnd);

	m_game = new CGameManager(m_hInst, m_hWnd);

	// D3D 초기화
	m_game->Initialize();
}



void CWinSetup::MsgLoop()
{
	// 메세지 루프 - Update(), Render()
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_game->Update();
			if (m_game != NULL)
				m_game->Render();
		}
	}

	UnregisterClass(L"3DFRAMEWORK_ver1.0", m_hInst);
}

void CWinSetup::Run()
{
	initWindow();
	createWindow();

	Init();
	
	MsgLoop();
}