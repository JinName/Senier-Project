#pragma once

#include <d3d9.h>
#include <dinput.h>
#include "Header.h"
#include "WinConfigure.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class CInput {
public:
	// 정적 싱글톤 구현
	static CInput* Get_Instance()
	{
		static CInput instance;
		return &instance;
	}

	bool InitDirectInput(HINSTANCE hInstance, HWND hwnd);

	bool IsKeyPressed(unsigned char key);
	bool IsMouseKeyPressed(unsigned char key);
	void GetMouseAbsolutePosition(long& x, long& y, long& z);
	void GetMouseRelativePosition(long& x, long& y, long& z);

	// 키다운 메세지
	bool Get_Keydown() { return m_bKeydown; }
	void Set_Keydown(bool _keydown) { m_bKeydown = _keydown; }

private:
	// 싱글톤 구현
	CInput();
	CInput(const CInput& other);
	~CInput();

	bool InitKeyboard(HWND hwnd);
	bool InitMouse(HWND hwnd);
	bool ReadKeyboardState();
	bool ReadMouseState();

private:
	// 윈도우 메세지로 키다운 판별
	bool m_bKeydown;

	HWND					m_hwnd;
	IDirectInput8*			m_ipDirectInput;
	LPDIRECTINPUTDEVICE8	m_ipKeyboard;
	LPDIRECTINPUTDEVICE8	m_ipMouse;

	unsigned char*			m_pKeyState;
	static const unsigned int
		m_iKeyStateLen;
	DIMOUSESTATE			m_sMouseState;
	long					m_lMouseX, m_lMouseY, m_lMouseZ;
};