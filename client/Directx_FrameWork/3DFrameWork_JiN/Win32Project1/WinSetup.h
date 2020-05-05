#pragma once

#include "Header.h"
#include "D3DSetup.h"
#include "GameManager.h"
#include "Camera.h"
#include "Util.h"

class CWinSetup
{
private:

	// ������ ����
	WNDCLASSEX m_wc;

	// ������ ����
	RECT m_rc;		// ������ ũ��
	HWND m_hWnd;
	HINSTANCE m_hInst;

	// ī�޶�
	CCamera m_Camera;

	// Ű����
	//CKeyBoard *m_keyboard;

	// ���� ���� - Game->Update(), Game->Rneder()
	//CD3DSetup *m_temp;
	//CGameManager *m_game;

	// editbox
	HWND m_hLoginButton;
	HWND m_hEditID;
	HWND m_hEditPW;
	WCHAR m_ID[64];
	WCHAR m_PW[64];

	// d3d ���� ���۽���ġ
	bool m_bD3DWork;

public:
	CWinSetup();
	CWinSetup(HINSTANCE hInstance);
	~CWinSetup() {};

	LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	// bool �� ������ �ʱ�ȭ�� ����� �ȵǾ����ÿ� �۵����� or ��� or �ٽ� �����õ�
	bool initWindow();
	void createWindow();

	// ������ ����, �ʿ��� �ʱ�ȭ
	VOID Init();

	// �޼��� ���� - Update() �� Render() �� ȣ��
	void MsgLoop();

	void Run();

	void CreateLoginWindow();


	// Ű���� �Է� ����
	
	// ���콺 �Է� ����
};

extern CWinSetup* g_pWinSetup;