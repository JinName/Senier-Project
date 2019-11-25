#pragma once
#include "Header.h"
#include "Character.h"
#include "Sprite.h"
#include "FireBall.h"

enum{ STAND, UP, DOWN, LEFT, ATTACK };

class CAru : public CCharacter
{
private:
	// 스탠드, 상 하 좌(우), 공격
	CSprite m_sprite[5];
	CSprite m_Run_Particle_Sprite;

	// FireBall Sprite 미리 로드해놓고 텍스처값만 넘겨주는 방식
	CSprite m_FireBall;
	CSprite m_FireBall_Hit;

	list<CFireBall*> m_FireBall_List;
	float m_fBefore_Clock;
	bool m_bAttack_Lock;
	float m_fAttack_Cooltime;

	// 캐릭터 속도
	float m_fSpeed;

	// 캐릭터 방향
	D3DXVECTOR2 m_vDirection;

	int m_iJump; // 2단 점프용
	bool m_bJump_Input_Lock;
	// 행동 변수
	bool m_bAttacking;
	bool m_bJump;
	bool m_b_isRunning;
	float m_fJump_Power;
	float m_fGravity_Accel;
	// 점프 용도
	// 점프 가능한 상태일때 true (바닥에 붙어있을때)
	bool m_bJump_is_Possible;
	// 점프시 충돌 가능상태
	bool m_bCollision_is_Possible;
	// 점프할 때 점프전 셋팅
	float m_fOld_Pos_y;
	bool m_bOld_Check;

	// 타일 충돌박스 밑에서 올라갈 경우
	// 충돌 준비가 됐을 경우 true;
	// 비교대상인 충돌박스보다 확실하게 위로 올라갔을때 true로
	bool m_bReady_to_Collision;

	// 마지막 움직인 방향 ( 좌우 스프라이트 반전용 )
	int currentDirection;

	// 애니매이션 번호
	int m_iAnimate_Num;
	// 애니매이션 마다 스프라이트 크기가 다르므로 RECT 재설정 필요

	// 중력계산용 시간
	DWORD dwOldtime;
	float m_fCharacter_mass; // 캐릭터 질량
	// 속력
	float velocity;

	// 충돌 시 캐릭터 움직임 제어
	bool isVertical; // 수직 충돌
	bool isHorizontal; // 수평 충돌
	bool isHit; // 피격

	int m_iHP; // 체력		
	bool m_bAfter_Collision_Setting; // 충돌 시 플레이어 상태 셋팅
	bool m_bActive_Collision; // 피격 후 무적상태
	float m_fCollision_Power; // 충돌시 밀려나는 힘

	// 포션과 충돌시
	bool m_bHP_isFull; // HP FULL 이면 true : 포션 못먹음
	bool m_bPotion_Collision; // 포션과 충돌시 true

public:
	CAru();
	~CAru();

	// HP 풀일때 true
	bool Get_HP_isFull();
	// 포션 충돌시 true
	void Set_Potion_Collision(bool _bPotion_Collision) { m_bPotion_Collision = _bPotion_Collision; }

	// 피격시 on/off : 몬스터와 충돌 가능 상태인지 반환
	bool Get_Active_Collision() { return m_bActive_Collision; }
	void Set_Active_Collision(bool _bActive_Collision) { m_bActive_Collision = _bActive_Collision; }

	// hp 반환
	int Get_HP() { return m_iHP; }

	// 파이어볼 리스트 반환 - 충돌처리
	list<CFireBall*> Get_FireBall_List() { return m_FireBall_List; }

	// 점프시 on/off 타일과 충돌 가능 상태인지 반환
	bool Get_Collision_is_Possible() { return m_bCollision_is_Possible; }
	// 충돌 시 변수 셋팅
	void Set_isVertical(bool _isVertical) { isVertical = _isVertical; }
	void Set_isHorizontal(bool _isHorizontal) { isHorizontal = _isHorizontal; }
	void Set_isHit(bool _isHit) { isHit = _isHit; }

	// 방향에 따른 애니매이션 번호 지정 함수
	void Set_Animation();
	// 캐릭터 최근 상태에 따른 DrawBitmap Reverse 여부
	bool Check_Reverse();

	// 캐릭터 중력
	void Gravity();
	// 캐릭터 점프
	void Jump();

	// 캐릭터가 타일과 충돌 시 움직임
	void isCrash_Tile();
	// 적과 부딪혔을때
	void isCrash_Enemy();
	// 포션과 충돌 시
	void isCrash_Potion();

	// 충돌 가능한 상태인지
	void Check_Collision_is_Possible();

	// FireBall Update()
	void Skill_Update();
	void Skill_Render();
	void Skill_Clean();

	// FireBall Destory
	void Skill_Destory();

	// Attack Cooltime
	void Attack_Cooltime();

	// KeyInput
	VOID KeyInput(LPDIRECT3DDEVICE9 _pDevice);

	void Init(LPDIRECT3DDEVICE9 _pDevice);
	void Update(LPDIRECT3DDEVICE9 _pDevice);
	void Render();
	void Clean();
};

