#pragma once
#include "Header.h"
#include "GameBase.h"
#include "DirectInput.h"
#include "BasicButton.h"
#include "ButtonManager.h"

class CTitle : public CGameBase
{
private:
	CBasicButton Button_GameStart;
	CBasicButton Button_Exit;
public:
	CTitle();
	~CTitle();

	// 부모의 함수
	virtual void OnInit(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnUpdate(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnRender(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnCleanup(LPDIRECT3DDEVICE9 _pDevice);
};

