#include "WinSetup.h"

CWinSetup::CWinSetup()
	:m_hWnd(NULL),
	m_hInst(NULL),
	m_game(NULL)
{
	// ������ ũ�� ����
	m_rc = { 0, 0, 640, 480 };
}

CWinSetup::CWinSetup(HINSTANCE hInstance)
	:m_hWnd(NULL),
	m_hInst(hInstance),
	m_game(NULL)
{
	// ������ ũ�� ����
	m_rc = { 0, 0, 960, 720 };	
}

// �������� ��ȭ�� �ݿ��ϱ� ���� MsgProc - �Ϲ� ��������� ���� ����
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


// ������ ������ ���� �Ѱ��ֱ� ���� ���������� ����� WndProc - �����Լ��̹Ƿ� �Ϲ� ��������� ���� �Ұ�
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
	//������ ����
	AdjustWindowRect(&m_rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindow(L"3DFRAMEWORK_ver1.0", L"3DFRAMEWORK", WS_OVERLAPPEDWINDOW,
		350, 120, m_rc.right - m_rc.left, m_rc.bottom - m_rc.top, NULL, NULL, m_hInst,
		NULL);

	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);	

	// ������ ���
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hWnd);
}

VOID CWinSetup::Init()
{
	// Ű���� ����
	CInput::Get_Instance()->InitDirectInput(m_hInst, m_hWnd);

	m_game = new CGameManager(m_hInst, m_hWnd);

	// D3D �ʱ�ȭ
	m_game->Initialize();
}



void CWinSetup::MsgLoop()
{
	// �޼��� ���� - Update(), Render()
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