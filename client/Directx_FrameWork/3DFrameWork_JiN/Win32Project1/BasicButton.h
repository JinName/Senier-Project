#pragma once
#include "Header.h"
#include "Sprite.h"

class CBasicButton
{
private:
	RECT m_rtButton; // ��ư ����
	CSprite m_Sprite; // ��ư �̹���
	
	float m_fButtonWidth; // ��ư ���α���
	float m_fButtonHeight; // ��ư ���α���
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

