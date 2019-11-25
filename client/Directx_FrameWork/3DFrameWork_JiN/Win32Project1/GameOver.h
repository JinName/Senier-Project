#pragma once
#include "Header.h"
#include "Sprite.h"
#include "GameBase.h"
#include "DirectInput.h"

class CGameOver : public CGameBase
{
private:
	CSprite m_GameOver_Sprite;

public:
	CGameOver();
	~CGameOver();

	virtual void OnInit(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnUpdate(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnRender(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnCleanup(LPDIRECT3DDEVICE9 _pDevice);
};

