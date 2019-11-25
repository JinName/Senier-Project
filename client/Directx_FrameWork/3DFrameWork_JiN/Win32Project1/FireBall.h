#pragma once
#include "Header.h"
#include "Sprite.h"
#include "Entity.h"

// 생성자에서 캐릭터의 위치를 받아 발사위치, 방향을 정함
class CFireBall : public CEntity
{
private:
	// 스프라이트
	CSprite m_FireBall;
	CSprite m_FireBall_Hit;

	float m_fDamage; // 데미지
	float m_fSpeed; // 투사체 속도
	int m_iDirection; // 발사 방향 ( 캐릭터 방향에 따른 )

	bool m_b_is_Collision; // 적과 충돌했을 경우 true
	bool m_bHit_Ani; // 사라지기전에 터지는 애니메이션
	bool m_bCollision_is_Possible; // 터지는 애니메이션일 때는 충돌 false

	bool m_bDestroy; // true 면 소멸

public:
	CFireBall();
	~CFireBall();

	bool Get_Collision_is_Possible() { return m_bCollision_is_Possible; }

	bool Get_isCollision() { return m_b_is_Collision; }
	void Set_isCollision(bool _isCollision) { m_b_is_Collision = _isCollision; }

	bool Get_isDestroy() { return m_bDestroy; }
	// 파이어볼 발사 위치 셋팅 (캐릭터 앞)
	// 기존 entity 클래스의 set_position 함수 이용

	void FireBall_Move();
	bool FireBall_Destroy_Check();

	bool Check_Reverse();

	// 미리 로드해놓은 스프라이트 셋팅
	void Set_Sprite(CSprite &_normalSprite, CSprite &_hitSprite);
	void Init(LPDIRECT3DDEVICE9 _pDevice, D3DXVECTOR3 _vPos, int _iDirection);
	void Update(); // 업데이트 함수에 캐릭터 상태를 받아오게끔, 발사 위치, 방향 등 설정
	void Render();
	void Clean();
};

