#pragma once
#include "Header.h"
#include "Sprite.h"

class CBasicButton
{
private:
	RECT m_rtButton; // 버튼 범위
	CSprite m_Sprite; // 버튼 이미지
	
	float m_fButtonWidth; // 버튼 가로길이
	float m_fButtonHeight; // 버튼 세로길이
public:
	CBasicButton();
	~CBasicButton();

	bool On_Click(float _fMouseX, float _fMouseY);

	RECT Get_Button_Rect() { return m_rtButton; }

	void Init(LPDIRECT3DDEVICE9 _pDevice, std::wstring _filename, float _fPosX, float _fPosY, float _fButtonWidth, float _fButtonHeight);
	void Update();
	void Render();
	void Clean();
};

