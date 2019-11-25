#pragma once

#include "Header.h"
#include "Sprite.h"
#include "Entity.h"

class CPotion : public CEntity
{
private:
	CSprite m_Sprite;

	//회복량
	int m_iRecoverPoint;

	// 충돌시 true
	bool m_b_is_Collision;

	// HP 다 떨어지면
	bool m_bDestroy;

protected:

public:
	CPotion();
	~CPotion();

	bool Get_Destroy() { return m_bDestroy; }

	bool Get_isCollision() { return m_b_is_Collision; }
	void Set_isCollision(bool _b_is_Collision) { m_b_is_Collision = _b_is_Collision; }

	int Get_RecoverPoint() { return m_iRecoverPoint; }

	// 포션 소멸
	void Potion_Destroy();

	void Init(LPDIRECT3DDEVICE9 _pDevice, D3DXVECTOR3 _vPos);
	void Update();
	void Render();
	void Clean();
};
