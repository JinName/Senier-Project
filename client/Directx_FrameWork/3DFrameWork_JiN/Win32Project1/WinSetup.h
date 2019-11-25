#pragma once

#include "Header.h"
#include "D3DSetup.h"
#include "GameManager.h"
#include "Camera.h"

class CWinSetup
{
private:

	// 윈도우 설정
	WNDCLASSEX m_wc;

	// 윈도우 생성
	RECT m_rc;		// 윈도우 크기
	HWND m_hWnd;
	HINSTANCE m_hInst;

	// 카메라
	CCamera m_Camera;

	// 키보드
	//CKeyBoard *m_keyboard;

	// 게임 정보 - Game->Update(), Game->Rneder()
	//CD3DSetup *m_temp;
	CGameManager *m_game;

public:
	CWinSetup();
	CWinSetup(HINSTANCE hInstance);
	~CWinSetup() {};

	LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	// bool 형 변수로 초기화가 제대로 안되었을시에 작동중지 or 경고 or 다시 생성시도
	bool initWindow();
	void createWindow();

	// 윈도우 제외, 필요한 초기화
	VOID Init();

	// 메세지 루프 - Update() 와 Render() 를 호출
	void MsgLoop();

	void Run();


	// 키보드 입력 리턴
	
	// 마우스 입력 리턴
};