//#include "PlayerInfo.h"
//
//void PlayerInfo::Init(int _iPlayerIndex)
//{
//	m_iHP = 3;
//	m_bHP_isFull = true;
//	m_iJump = 0;
//	m_fSpeed = 3.0f;
//	m_fJump_Power = 0.0f;
//	m_fGravity_Accel = 0.0f;
//	m_vDirection = { 0.0f, 0.0f };
//	// 캐릭 질량
//	m_fCharacter_mass = 4.0f;
//	m_fCollision_Power = 15.0f; // 충돌시 밀려나는 힘
//
//	isVertical = false; // 수직 충돌
//	isHorizontal = false; // 수평 충돌
//	isHit = false; // 피격
//
//	// 충돌박스 선 그리기
//	//Line_Init(_pDevice);
//
//	// 캐릭터 기본 시작 위치
//	if (_iPlayerIndex == 0)
//		m_vPos = { 100.0f, 300.0f, 0.0f };
//	else if (_iPlayerIndex == 1)
//		m_vPos = { 600.0f, 300.0f, 0.0f };
//
//	// 몬스터와 충돌시 false : 입력도 받지않고 충돌도 하지않는 무적상태 3초
//	m_bActive_Collision = true;
//	m_bAfter_Collision_Setting = false; // 몬스터와 충돌시 셋팅
//
//	// 공격 중인지
//	m_bAttacking = false;
//
//	// 점프 관련
//	m_bJump_Input_Lock = false;
//	m_bJump_is_Possible = false; // 바닥에 붙어있을때만 true
//	m_bCollision_is_Possible = true; // 점프 중일때 false
//	m_b_isRunning = false;
//	m_bJump = false;
//	m_fOld_Pos_y = 0.0f;
//	m_bOld_Check = false;
//
//	// 마지막 움직인 방향 ( 좌우 스프라이트 반전용 )
//	currentDirection = 1;
//
//	// 애니매이션 넘버 ( 기본 = 0 )
//	m_iAnimate_Num = 0;
//
//	dwOldtime = GetTickCount();
//	velocity = 0.0f;
//
//	//FireBall Cooltime
//	m_fAttack_Cooltime = 1.0f;
//	m_bAttack_Lock = false;
//	m_fBefore_Clock = 0.0f;
//}
//
//void PlayerInfo::SetPosition(float _x, float _y, float _z)
//{
//	m_vPos.x = _x;
//	m_vPos.y = _y;
//	m_vPos.z = _z;
//}
//
//void PlayerInfo::Do_Left()
//{
//	m_vPos.x -= m_fSpeed;
//	m_vDirection.x = -1.0f;
//	m_b_isRunning = true;
//	currentDirection = -1;
//}
//
//void PlayerInfo::Do_Right()
//{
//	m_vPos.x += m_fSpeed;
//	m_vDirection.x = 1.0f;
//	m_b_isRunning = true;
//	currentDirection = 1;
//}