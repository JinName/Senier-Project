#include "Player.h"

void Player::Do_Stand()
{
	m_vDirection = { 0.0f, 0.0f };
	m_b_isRunning = false;
	//m_Run_Particle_Sprite.Reset_Sprite();
}

void Player::Do_Attack()
{
	/*CFireBall* FireBall = new CFireBall();
	FireBall->Init(m_vPos, currentDirection);
	FireBall->Set_Sprite(m_FireBall, m_FireBall_Hit);
	m_FireBall_List.push_back(FireBall);

	m_fBefore_Clock = clock();

	m_bAttacking = true;
	m_bAttack_Lock = true;*/
}

void Player::Do_Left()
{
	m_vPos.x -= m_fSpeed;
	m_vDirection.x = -1.0f;
	m_b_isRunning = true;
	currentDirection = -1;
}

void Player::Do_Right()
{
	m_vPos.x += m_fSpeed;
	m_vDirection.x = 1.0f;
	m_b_isRunning = true;
	currentDirection = 1;
}

void Player::Do_Jump()
{
	if (m_iJump == 0)
	{
		if (!m_bOld_Check)
		{
			m_bJump = true;
			++m_iJump;
		}
	}
	else if (m_iJump == 1)
	{
		m_bOld_Check = false;
		++m_iJump;
	}
	m_bJump_Input_Lock = true;
}

void Player::Do_Not_Jump()
{
	m_bJump_Input_Lock = false;
}