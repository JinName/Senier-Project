#pragma once
#include "Header.h"
#include "BasicButton.h"

class CButtonManager
{
private:
	RECT m_rt;

	CBasicButton Button_GameStart;
	//CBasicButton Button_Exit;
public:
	CButtonManager();
	~CButtonManager();

	// 버튼 생성
	void Init(LPDIRECT3DDEVICE9 _pDevice);
	void Update(float _fMouseX, float _fMouseY);
	void Render();
	void Clean();
};

