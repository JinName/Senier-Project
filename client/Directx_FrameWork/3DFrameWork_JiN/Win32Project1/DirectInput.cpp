#include "DirectInput.h"

const unsigned int CInput::m_iKeyStateLen = 256;

CInput::CInput()
	:m_ipDirectInput(NULL)
	,m_bKeydown(false)
{
	m_pKeyState = new unsigned char[m_iKeyStateLen];
	m_lMouseX = 0;
	m_lMouseY = 0;
	m_lMouseZ = 0;
	ZeroMemory(&m_sMouseState, sizeof(m_sMouseState));
}

CInput::~CInput()
{
	if (m_ipDirectInput != NULL)
	{
		m_ipDirectInput->Release();
		m_ipDirectInput = NULL;
	}
	if (m_ipKeyboard != NULL)
	{
		m_ipKeyboard->Unacquire();
		m_ipKeyboard->Release();
		m_ipKeyboard = NULL;
	}
	if (m_ipMouse != NULL)
	{
		m_ipMouse->Unacquire();
		m_ipMouse->Release();
		m_ipMouse = NULL;
	}
	delete[] m_pKeyState;	
	m_pKeyState = NULL;
}

bool CInput::InitDirectInput(HINSTANCE hInstance, HWND hwnd)
{
	m_hwnd = hwnd;

	if (FAILED(DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(LPVOID*)&m_ipDirectInput,
		NULL)))
	{
		MessageBox(hwnd,
			L"DirectInput8Create() failed!",
			L"ERROR",
			MB_OK);
		return false;
	}
	if (!InitKeyboard(hwnd))
		return false;

	if (!InitMouse(hwnd))
		return false;

	return true;
}
bool CInput::InitKeyboard(HWND hwnd)
{
	if (FAILED(m_ipDirectInput->CreateDevice(GUID_SysKeyboard, &m_ipKeyboard, NULL)))
	{
		MessageBox(hwnd,
			L"Create Keyboard Failed!",
			L"ERROR",
			MB_OK);
		return false;
	}

	if (FAILED(m_ipKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hwnd,
			L"Set Keyboard Data Format Failed!",
			L"ERROR",
			MB_OK);
		return false;
	}

	if (FAILED(m_ipKeyboard->SetCooperativeLevel(hwnd,
		DISCL_FOREGROUND | 
		DISCL_EXCLUSIVE))) 
	{
		MessageBox(hwnd,
			L"Set Keyboard Cooperative Level Failed!",
			L"ERROR",
			MB_OK);
		return false;
	}
	
	if (FAILED(m_ipKeyboard->Acquire()))
	{
		MessageBox(hwnd,
			L"Acquire Keyboard Failed!",
			L"ERROR",
			MB_OK);
		return false;
	}
	return true;
}

bool CInput::InitMouse(HWND hwnd)
{
	m_hwnd = hwnd;

	if (FAILED(m_ipDirectInput->CreateDevice(GUID_SysMouse, &m_ipMouse, NULL)))
	{
		MessageBox(hwnd,
			L"Create Mouse Failed!",
			L"ERROR",
			MB_OK);
		return false;
	}

	if (FAILED(m_ipMouse->SetDataFormat(&c_dfDIMouse)))
	{
		MessageBox(hwnd,
			L"Set Mouse Data Format Failed!",
			L"ERROR",
			MB_OK);
		return false;
	}

	if (FAILED(m_ipMouse->SetCooperativeLevel(hwnd,
		DISCL_FOREGROUND |
		DISCL_NONEXCLUSIVE)))
	{
		MessageBox(hwnd,
			L"Set Mouse Cooperative Level Failed!",
			L"ERROR",
			MB_OK);
		return false;
	}

	if (FAILED(m_ipMouse->Acquire()))
	{
		MessageBox(hwnd,
			L"Acquire Mouse Failed!",
			L"ERROR",
			MB_OK);
		return false;
	}
	return true;
}

bool CInput::IsKeyPressed(unsigned char key)
{
	if (!ReadKeyboardState())
		return false;
	else
		return ((m_pKeyState[key] & 0x80) ? true : false);
}

bool CInput::ReadKeyboardState()
{
	HRESULT  hr = m_ipKeyboard->GetDeviceState(m_iKeyStateLen, (LPVOID)m_pKeyState);
	if (FAILED(hr))
	{
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
			m_ipKeyboard->Acquire();
		else
			return false;
	}
	return true;
}

bool CInput::IsMouseKeyPressed(unsigned char key)
{
	if (!ReadMouseState())
		return false;
	else
		return ((m_sMouseState.rgbButtons[key] & 0x80) ? true : false);
}
void CInput::GetMouseAbsolutePosition(long& x, long& y, long& z)
{
	ReadMouseState();
	
	m_lMouseX += m_sMouseState.lX;
	m_lMouseY += m_sMouseState.lY;
	m_lMouseZ += m_sMouseState.lZ;

	if (m_lMouseX < 0)  m_lMouseX = 0;
	if (m_lMouseY < 0)  m_lMouseY = 0;

	if (m_lMouseX > WND_WIDTH)  m_lMouseX = WND_WIDTH;
	if (m_lMouseY > WND_HEIGHT) m_lMouseY = WND_HEIGHT;

	x = m_lMouseX;
	y = m_lMouseY;
	z = m_lMouseZ;
}
void CInput::GetMouseRelativePosition(long& x, long& y, long& z)
{
	//ReadMouseState();
	//x = m_sMouseState.lX;
	//y = m_sMouseState.lY;
	//z = m_sMouseState.lZ;
	POINT	cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(m_hwnd, &cursorPos);
	x = cursorPos.x;
	y = cursorPos.y;
}
bool CInput::ReadMouseState()
{
	HRESULT  hr = m_ipMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_sMouseState);
	if (FAILED(hr))
	{
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
			m_ipMouse->Acquire();
		else
			return false;
	}
	return true;
}