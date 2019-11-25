#pragma once
#include "Header.h"
#include "Sprite.h"
#include "Entity.h"

class CHealth : public CEntity
{
private:
	// Health Texture
	CSprite m_Sprite[3];

	// 외부에서 초기화하는 HP에 따라 텍스쳐 늘어남
	// HP 텍스쳐 갯수
	int m_iHP_Texture_Count;
	// 실제 HP
	int m_iHP;

public:
	CHealth();
	~CHealth();

	void Set_HP(int _iHP) { m_iHP = _iHP; }

	void Init(LPDIRECT3DDEVICE9 _pDevice, int _iHP, D3DXVECTOR3 _vPos);
	void Update();
	void Render();
	void Clean();
};

