#pragma once
#include "Header.h"
#include "Entity.h"
#include "Sprite.h"

// 뒷 배경은 센터를 잡지않고 그냥 왼쪽 위를 기준으로 그릴것
class CBackground :public CEntity
{
private:
	CSprite m_Sprite;
public:
	CBackground();
	~CBackground();

	void Init(LPDIRECT3DDEVICE9 _pDevice);
	void Update();
	void Render();
	void Clean();
};

