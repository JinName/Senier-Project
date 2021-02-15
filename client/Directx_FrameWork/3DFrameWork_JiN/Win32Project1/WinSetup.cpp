#include "WinSetup.h"
#include "Crypt.h"

CWinSetup* g_pWinSetup = nullptr;

CWinSetup::CWinSetup()
	:m_hWnd(NULL),
	m_hInst(NULL),
	m_bD3DWork(false),
	m_hEditID(NULL),
	m_hEditPW(NULL)
	//m_game(NULL)
{
	// ������ ũ�� ����
	m_rc = { 0, 0, 640, 480 };
}

CWinSetup::CWinSetup(HINSTANCE hInstance)
	:m_hWnd(NULL),
	m_hInst(hInstance),
	m_bD3DWork(false),
	m_hEditID(NULL),
	m_hEditPW(NULL)
	//m_game(NULL)
{
	// ������ ũ�� ����
	m_rc = { 0, 0, 960, 720 };
}

// �������� ��ȭ�� �ݿ��ϱ� ���� MsgProc - �Ϲ� ��������� ���� ����
LRESULT WINAPI CWinSetup::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		

		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 11:
		{
			// LOGIN ��ư �������� ��Ŷ �����Ͽ� ���̵� ��� Ȯ��
			GetWindowText(m_hEditID, m_ID, 64);
			GetWindowText(m_hEditPW, m_PW, 64);

			char* id = ConvertWCtoC(m_ID);
			char* pw = ConvertWCtoC(m_PW);

			g_pCrypt->Encrypt(pw);

			SLOGIN login;
			memset(&login, 0, sizeof(SLOGIN));
			strcpy(login.mID, id);
			strcpy(login.mPW, pw);

			g_pNetwork->SendPacket(PROTOCOL::LOGIN_RQ, (char*)&login, sizeof(SLOGIN), false);

			delete[] id;
			delete[] pw;

			break;
		}
		}
		return 0;

	case WM_DESTROY:
		if (m_bD3DWork)
		{
			g_pGameManager->Cleanup();
			delete g_pGameManager;
			g_pGameManager = NULL;
		}

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
	//AllocConsole();
	//freopen("CONOUT$", "wt", stdout);

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
	// login ������ Editbox ����
	ShowWindow(m_hLoginButton, SW_HIDE);
	ShowWindow(m_hEditID, SW_HIDE);
	ShowWindow(m_hEditPW, SW_HIDE);

	// Ű���� ����
	CInput::Get_Instance()->InitDirectInput(m_hInst, m_hWnd);

	g_pGameManager = new CGameManager(m_hInst, m_hWnd);

	// D3D �ʱ�ȭ
	g_pGameManager->Initialize();

	m_bD3DWork = true;
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
			if (m_bD3DWork) // �α��� ���� -> init() -> m_bD3DWork = true;
			{
				g_pGameManager->Update();
				if (g_pGameManager != NULL)
					g_pGameManager->Render();

				Sleep(16);
			}			
		}
	}

	UnregisterClass(L"3DFRAMEWORK_ver1.0", m_hInst);
}

void CWinSetup::Run()
{
	initWindow();
	createWindow();

	CreateLoginWindow();
	//Init();
	
	MsgLoop();
}

void CWinSetup::CreateLoginWindow()
{
	m_hEditID = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 300, 100, 100, 30, m_hWnd, (HMENU)21, m_hInst, NULL);
	m_hEditPW = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD | EM_SETPASSWORDCHAR, 300, 200, 100, 30, m_hWnd, (HMENU)22, m_hInst, NULL);
	// LOGIN BUTTON
	m_hLoginButton = CreateWindow(L"button", L"LOGIN", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 500, 100, 100, 30, m_hWnd, (HMENU)11, m_hInst, NULL);

	SendMessage(m_hEditPW, EM_SETPASSWORDCHAR, (WPARAM)'*', 0);

	UpdateWindow(m_hWnd);
}