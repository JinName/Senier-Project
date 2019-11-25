#pragma once
#include "Header.h"
#include "Entity.h"
#include "Sprite.h"

class CMonster : public CEntity
{
private:
	CSprite m_Sprite;
	CSprite m_Monster_Hit; // 몬스터 피격 스프라이트
	bool m_bHit_Ani; // 피격 애니메이션

	// HP
	int m_iHP;

	// 좌우 최대 최소 이동 범위
	float m_fMax_x;
	float m_fMin_x;

	// 좌 우 처음 이동방향 랜덤
	float m_iDirection;

	// 이동 속도
	float m_fSpeed;

	// 충돌했을 때
	bool m_b_is_Collision;
	int m_iCollision_Direction; // 충돌 방향
	float m_fCollision_Power; // 충돌시 밀려나는 힘

	// HP 다 떨어지면
	bool m_bDestroy;
public:
	CMonster();
	~CMonster();

	void Set_Collision_Direction(int _iCollision_Direction) { m_iCollision_Direction = _iCollision_Direction; }

	bool Get_Destroy() { return m_bDestroy; }

	bool Get_isCollision() { return m_b_is_Collision; }
	void Set_isCollision(bool _b_is_Collision) { m_b_is_Collision = _b_is_Collision; }

	// 몬스터 좌우 움직임
	void Monster_Move();
	// 몬스터 충돌 움직임
	void Monster_Collision_Move();

	// 충돌시
	void Monster_Collision_Check();

	// 몬스터 소멸
	void Monster_Destroy();

	void Init(LPDIRECT3DDEVICE9 _pDevice, D3DXVECTOR3 _vPos, float _fMove_Width);
	void Update();
	void Render();
	void Clean();
};

