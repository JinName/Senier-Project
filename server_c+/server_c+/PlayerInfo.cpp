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
//	// ĳ�� ����
//	m_fCharacter_mass = 4.0f;
//	m_fCollision_Power = 15.0f; // �浹�� �з����� ��
//
//	isVertical = false; // ���� �浹
//	isHorizontal = false; // ���� �浹
//	isHit = false; // �ǰ�
//
//	// �浹�ڽ� �� �׸���
//	//Line_Init(_pDevice);
//
//	// ĳ���� �⺻ ���� ��ġ
//	if (_iPlayerIndex == 0)
//		m_vPos = { 100.0f, 300.0f, 0.0f };
//	else if (_iPlayerIndex == 1)
//		m_vPos = { 600.0f, 300.0f, 0.0f };
//
//	// ���Ϳ� �浹�� false : �Էµ� �����ʰ� �浹�� �����ʴ� �������� 3��
//	m_bActive_Collision = true;
//	m_bAfter_Collision_Setting = false; // ���Ϳ� �浹�� ����
//
//	// ���� ������
//	m_bAttacking = false;
//
//	// ���� ����
//	m_bJump_Input_Lock = false;
//	m_bJump_is_Possible = false; // �ٴڿ� �پ��������� true
//	m_bCollision_is_Possible = true; // ���� ���϶� false
//	m_b_isRunning = false;
//	m_bJump = false;
//	m_fOld_Pos_y = 0.0f;
//	m_bOld_Check = false;
//
//	// ������ ������ ���� ( �¿� ��������Ʈ ������ )
//	currentDirection = 1;
//
//	// �ִϸ��̼� �ѹ� ( �⺻ = 0 )
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