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

void Player::Set_Animation()
{
	if (m_bAttacking == false)
	{
		if (m_vDirection.x == 0 && m_vDirection.y == 0)
		{
			m_iAnimate_Num = 0;
		}
		else if (m_vDirection.y < 0)
		{
			m_iAnimate_Num = 1;
		}
		else if (m_vDirection.y > 0)
		{
			m_iAnimate_Num = 2;
		}
		else if (m_vDirection.x > 0 || m_vDirection.x < 0)
		{
			m_iAnimate_Num = 3;
		}
	}
	else if (m_bAttacking == true)
	{
		m_iAnimate_Num = 4;
	}
}

void Player::Set_Animation(int _iAnimate_Num)
{
	if (!m_bAttacking)
	{
		if (_iAnimate_Num == 6)
			m_iAnimate_Num = 3;
		else
			m_iAnimate_Num = _iAnimate_Num;
	}

	switch (_iAnimate_Num)
	{
	case STAND:
		m_vDirection = { 0.0f, 0.0f };
		m_b_isRunning = false;
		break;

	case LEFT:
		m_vDirection.x = -1.0f;
		m_b_isRunning = true;
		currentDirection = -1;
		break;

	case RIGHT:
		m_vDirection.x = 1.0f;
		m_b_isRunning = true;
		currentDirection = 1;
		break;

	case ATTACK:
		//CFireBall* FireBall = new CFireBall();
		//FireBall->Init(m_vPos, currentDirection);
		//FireBall->Set_Sprite(m_FireBall, m_FireBall_Hit);
		//m_FireBall_List.push_back(FireBall);

		//m_fBefore_Clock = clock();

		//m_bAttacking = true;
		//m_bAttack_Lock = true;
		break;
	}
}

bool Player::Check_Reverse()
{
	if (m_vDirection.x == 0)
	{
		if (currentDirection == -1)
			return false;
		else if (currentDirection == 1)
			return true;
	}
	else if (m_vDirection.x > 0 || m_vDirection.x < 0)
	{
		if (m_vDirection.x < 0)
			return false;
		else if (m_vDirection.x > 0)
			return true;
	}

	return false;
}

void Player::Jump()
{
	if (m_bJump)
	{
		if (m_iJump == 1)
		{
			if (!m_bOld_Check)
			{
				//m_fOld_Pos_y = m_vPos.y;
				m_bCollision_is_Possible = false;
				m_bOld_Check = true;
			}
			m_vPos.y -= m_fJump_Power;
		}
		else if (m_iJump == 2)
		{
			if (!m_bOld_Check)
			{
				//m_fOld_Pos_y = m_vPos.y;
				m_bCollision_is_Possible = false;
				m_fGravity_Accel = 0.0f;
				velocity = 0.0f;
				m_bOld_Check = true;
			}
			m_vPos.y -= m_fJump_Power;
		}
	}
}

void Player::Gravity()
{
	DWORD currentTime = GetTickCount();

	DWORD TempTime = currentTime - dwOldtime;

	dwOldtime = currentTime;

	if (!isVertical)
	{
		velocity += 0.0098f * (float)TempTime / 60.0f * 2.0f;
		m_fGravity_Accel = velocity * (float)TempTime * m_fCharacter_mass;
		m_vPos.y = m_vPos.y + m_fGravity_Accel;
	}
	else
	{
		m_fGravity_Accel = 0.0f;
		velocity = 0.0f;
	}
}

void Player::isCrash_Tile()
{
	if (isVertical)
	{
		m_vPos.y = m_vPos.y;
		m_bJump = false;
		m_iJump = 0;
		m_bOld_Check = false;
		m_bJump_is_Possible = true;
	}
	else
	{
		m_bJump_is_Possible = false;
	}
	/*
	else if (isHorizontal)
	{
		m_vPos.x = m_vPos.x;
		m_bJump = false;
		m_bOld_Check = false;
	}
	*/
}

void Player::isCrash_Enemy()
{
	if (m_bActive_Collision == false)
	{
		if (m_bAfter_Collision_Setting == false) // ü�°���
		{
			m_iHP -= 1;
			m_bJump = false;// ���� �ߴ�
			m_bCollision_is_Possible = true; // Ÿ�ϰ��� �浹 ����
			m_bAfter_Collision_Setting = true;
		}
		// �з���
		m_vPos.x = m_vPos.x + -currentDirection * m_fCollision_Power;
		m_fCollision_Power -= 1.0f;
		// ������� �з����� ���Ϳ� �浹 ���� ���·� ��ȯ
		if (m_fCollision_Power < 0)
		{
			m_bAfter_Collision_Setting = false;
			m_fCollision_Power = 10.0f;
			m_bActive_Collision = true;
		}
	}
}

void Player::isCrash_Potion()
{
	if (m_bPotion_Collision == true)
	{
		m_iHP += 1;

		m_bPotion_Collision = false;
	}
}

bool Player::Get_HP_isFull()
{
	if (m_iHP == 3)
		return true;
	else
		return false;
}

void Player::Check_Collision_is_Possible()
{
	if (m_bJump)
	{
		if (m_fGravity_Accel > m_fJump_Power)
		{
			m_bCollision_is_Possible = true;
		}
	}
}

//void Player::Skill_Update()
//{
//	if (m_FireBall_List.size() > 0)
//	{
//		for (list<CFireBall*>::iterator begin_iter = m_FireBall_List.begin();
//			begin_iter != m_FireBall_List.end(); begin_iter++)
//		{
//			(*begin_iter)->Update();
//		}
//	}
//}

//void Player::Skill_Clean()
//{
//	if (m_FireBall_List.size() > 0)
//	{
//		for (list<CFireBall*>::iterator begin_iter = m_FireBall_List.begin();
//			begin_iter != m_FireBall_List.end(); begin_iter++)
//		{
//			delete (*begin_iter);
//		}
//		m_FireBall_List.clear();
//	}
//}

//void Player::Skill_Destory()
//{
//	if (m_FireBall_List.size() > 0)
//	{
//		for (list<CFireBall*>::iterator begin_iter = m_FireBall_List.begin();
//			begin_iter != m_FireBall_List.end(); )
//		{
//			if ((*begin_iter)->FireBall_Destroy_Check() == true)
//			{
//				begin_iter = m_FireBall_List.erase(begin_iter);
//			}
//			else
//				++begin_iter;
//		}
//	}
//}


void Player::Attack_Cooltime()
{
	if (m_bAttack_Lock == true)
	{
		if ((float)((clock() - m_fBefore_Clock) / CLOCKS_PER_SEC) >= m_fAttack_Cooltime)
		{
			m_bAttack_Lock = false;
		}
	}
}


////////////// Init, Update, Render, Clean  /////////////////////////////////////////////

void Player::Init()
{
	m_iHP = 3;
	m_bHP_isFull = true;
	m_iJump = 0;
	m_fSpeed = 3.0f;
	m_fJump_Power = 4.5f;
	m_fGravity_Accel = 0.0f;
	m_vDirection = { 0.0f, 0.0f };
	// ĳ�� ����
	m_fCharacter_mass = 4.0f;
	m_fCollision_Power = 15.0f; // �浹�� �з����� ��

	isVertical = false; // ���� �浹
	isHorizontal = false; // ���� �浹
	isHit = false; // �ǰ�

	// �浹�ڽ� �� �׸���
	//Line_Init(_pDevice);

	// ĳ���� �⺻ ���� ��ġ
	m_vPos = { 100.0f, 300.0f, 0.0f };

	// ���Ϳ� �浹�� false : �Էµ� �����ʰ� �浹�� �����ʴ� �������� 3��
	m_bActive_Collision = true;
	m_bAfter_Collision_Setting = false; // ���Ϳ� �浹�� ����

	// ���� ������
	m_bAttacking = false;

	// ���� ����
	m_bJump_Input_Lock = false;
	m_bJump_is_Possible = false; // �ٴڿ� �پ��������� true
	m_bCollision_is_Possible = true; // ���� ���϶� false
	m_b_isRunning = false;
	m_bJump = false;
	m_fOld_Pos_y = 0.0f;
	m_bOld_Check = false;

	// ������ ������ ���� ( �¿� ��������Ʈ ������ )
	currentDirection = 1;

	// �ִϸ��̼� �ѹ� ( �⺻ = 0 )
	m_iAnimate_Num = 0;

	dwOldtime = GetTickCount();
	velocity = 0.0f;

	//FireBall Cooltime
	m_fAttack_Cooltime = 1.0f;
	m_bAttack_Lock = false;
	m_fBefore_Clock = 0.0f;
}

void Player::Update()
{
	isCrash_Tile();
	isCrash_Enemy();
	isCrash_Potion();

	Check_Collision_is_Possible(); // ���� �ÿ� Ÿ�ϰ� �浹 ���ɻ������� Ȯ��

	if (m_bActive_Collision == true)
	{
		Jump();
	}

	//if (m_bIsPlayer)
		//Gravity();

	Attack_Cooltime();
	//Skill_Update();
	//Skill_Destory();

	Set_Animation();

	//m_sprite[m_iAnimate_Num].Animation_Frame();

	/*if (m_iAnimate_Num == 4 && m_sprite[m_iAnimate_Num].Get_Last_Frame() == true)
	{
		m_bAttacking = false;
	}*/

	/*if (m_b_isRunning && !m_bJump)
	{
		m_Run_Particle_Sprite.Animation_Frame();
	}*/

	//Set_Collider(m_sprite[m_iAnimate_Num].Get_Sprite_Width() - 20.0f, m_sprite[m_iAnimate_Num].Get_Sprite_Height(), true, RECT{0, 20, 0, 0});
	SetCollider(44.0f, 64.0f, true, RECT{ 0, 20, 0, 0 });
}

void Player::Clean()
{
	/*for (int i = 0; i < 5; ++i)
	{
		m_sprite[i].CleanUp();
	}

	m_Run_Particle_Sprite.CleanUp();
	Skill_Clean();
	m_FireBall.CleanUp();
	m_FireBall_Hit.CleanUp();*/
}