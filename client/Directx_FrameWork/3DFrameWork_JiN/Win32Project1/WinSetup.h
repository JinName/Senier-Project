#pragma once

#include "Header.h"
#include "D3DSetup.h"
#include "GameManager.h"
#include "Camera.h"

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
	CGameManager *m_game;

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


	// Ű���� �Է� ����
	
	// ���콺 �Է� ����
};