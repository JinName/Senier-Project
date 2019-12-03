#pragma once
#include "stdafx.h"

struct VECTOR2
{
	float x;
	float y;
};

struct VECTOR3
{
	float x;
	float y;
	float z;
};

class PlayerInfo
{
public:
	PlayerInfo();
	~PlayerInfo();

	void Init(int _iPlayerIndex);

	// do function
	void Do_Left();
	void Do_Right();

private:
	// 캐릭터 위치
	VECTOR3 m_vPos;

	float m_fBefore_Clock;
	bool m_bAttack_Lock;
	float m_fAttack_Cooltime;

	// 캐릭터 속도
	float m_fSpeed;

	// 캐릭터 방향
	VECTOR2 m_vDirection;

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
};

