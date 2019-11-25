#pragma once
#include "Header.h"
#include "Sprite.h"
#include "GameBase.h"

class CGameClear : public CGameBase
{
private:
	CSprite m_GameClear_Sprite;

public:
	CGameClear();
	~CGameClear();

	virtual void OnInit(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnUpdate(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnRender(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnCleanup(LPDIRECT3DDEVICE9 _pDevice);
};

