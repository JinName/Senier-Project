#include "Aru.h"



CAru::CAru()
{
}

CAru::~CAru()
{
}

void CAru::Set_Animation()
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

bool CAru::Check_Reverse()
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

void CAru::Jump()
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

void CAru::Gravity()
{	
	DWORD currentTime = GetTickCount();

	DWORD TempTime = currentTime - dwOldtime;
		
	dwOldtime = currentTime;

	if (!isVertical)
	{
		velocity += 0.0098f * (float)TempTime / 60.0f;
		m_fGravity_Accel = velocity * (float)TempTime * m_fCharacter_mass;
		m_vPos.y = m_vPos.y + m_fGravity_Accel;
	}
	else
	{
		m_fGravity_Accel = 0.0f;
		velocity = 0.0f;
	}
}

void CAru::isCrash_Tile()
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

void CAru::isCrash_Enemy()
{
	if (m_bActive_Collision == false)
	{
		if (m_bAfter_Collision_Setting == false) // 체력감소
		{
			m_iHP -= 1;
			m_bJump = false;// 점프 중단
			m_bCollision_is_Possible = true; // 타일과는 충돌 가능
			m_bAfter_Collision_Setting = true;
		}
		// 밀려남
		m_vPos.x = m_vPos.x + -currentDirection * m_fCollision_Power;
		m_fCollision_Power -= 1.0f;
		// 어느정도 밀려나면 몬스터와 충돌 가능 상태로 전환
		if (m_fCollision_Power < 0)
		{
			m_bAfter_Collision_Setting = false;
			m_fCollision_Power = 10.0f;
			m_bActive_Collision = true;
		}
	}
}

void CAru::isCrash_Potion()
{
	if (m_bPotion_Collision == true)
	{
		m_iHP += 1;

		m_bPotion_Collision = false;
	}
}

bool CAru::Get_HP_isFull()
{
	if (m_iHP == 3)
		return true;
	else
		return false;
}

void CAru::Check_Collision_is_Possible()
{
	if (m_bJump)
	{
		if (m_fGravity_Accel > m_fJump_Power)
		{
			m_bCollision_is_Possible = true;
		}
	}
}

void CAru::Skill_Update()
{
	if (m_FireBall_List.size() > 0)
	{
		for (list<CFireBall*>::iterator begin_iter = m_FireBall_List.begin();
			begin_iter != m_FireBall_List.end(); begin_iter++)
		{
			(*begin_iter)->Update();
		}
	}
}

void CAru::Skill_Render()
{
	if (m_FireBall_List.size() > 0)
	{
		for (list<CFireBall*>::iterator begin_iter = m_FireBall_List.begin();
			begin_iter != m_FireBall_List.end(); begin_iter++)
		{
			(*begin_iter)->Render();
		}
	}
}

void CAru::Skill_Clean()
{
	if (m_FireBall_List.size() > 0)
	{
		for (list<CFireBall*>::iterator begin_iter = m_FireBall_List.begin();
			begin_iter != m_FireBall_List.end(); begin_iter++)
		{
			delete (*begin_iter);
		}
		m_FireBall_List.clear();
	}
}

void CAru::Skill_Destory()
{
	if (m_FireBall_List.size() > 0)
	{
		for (list<CFireBall*>::iterator begin_iter = m_FireBall_List.begin();
			begin_iter != m_FireBall_List.end(); )
		{
			if ((*begin_iter)->FireBall_Destroy_Check() == true)
			{
				begin_iter = m_FireBall_List.erase(begin_iter);
			}
			else
				++begin_iter;
		}
	}
}


void CAru::Attack_Cooltime()
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

void CAru::Init(LPDIRECT3DDEVICE9 _pDevice)
{
	m_iHP = 3;
	m_bHP_isFull = true;
	m_iJump = 0;
	m_fSpeed = 3.0f;
	m_fJump_Power = 6.2f;
	m_fGravity_Accel = 0.0f;
	m_vDirection = { 0.0f, 0.0f };
	// 캐릭 질량
	m_fCharacter_mass = 4.0f;
	m_fCollision_Power = 15.0f; // 충돌시 밀려나는 힘

	isVertical = false; // 수직 충돌
	isHorizontal = false; // 수평 충돌
	isHit = false; // 피격

	// 충돌박스 선 그리기
	//Line_Init(_pDevice);

	// 캐릭터 기본 시작 위치
	m_vPos = { 100.0f, 300.0f, 0.0f };

	// 몬스터와 충돌시 false : 입력도 받지않고 충돌도 하지않는 무적상태 3초
	m_bActive_Collision = true;
	m_bAfter_Collision_Setting = false; // 몬스터와 충돌시 셋팅

	// 공격 중인지
	m_bAttacking = false;

	// 점프 관련
	m_bJump_Input_Lock = false;
	m_bJump_is_Possible = false; // 바닥에 붙어있을때만 true
	m_bCollision_is_Possible = true; // 점프 중일때 false
	m_b_isRunning = false;
	m_bJump = false;
	m_fOld_Pos_y = 0.0f;
	m_bOld_Check = false;

	// 마지막 움직인 방향 ( 좌우 스프라이트 반전용 )
	currentDirection = 1;

	// 애니매이션 넘버 ( 기본 = 0 )
	m_iAnimate_Num = 0;

	dwOldtime = GetTickCount();
	velocity = 0.0f;

	//FireBall Cooltime
	m_fAttack_Cooltime = 1.0f;
	m_bAttack_Lock = false;
	m_fBefore_Clock = 0.0f;
	
	// 캐릭터 스탠드 스프라이트
	m_sprite[STAND].Create_Sprite(_pDevice, L"2D_Sprites\\Aru_stand_8peaces.bmp", 512, 64, 8, D3DCOLOR_XRGB(0, 170, 255));
	m_sprite[UP].Create_Sprite(_pDevice, L"2D_Sprites\\Aru_up_8peaces.bmp", 512, 64, 8, D3DCOLOR_XRGB(0, 170, 255));
	m_sprite[DOWN].Create_Sprite(_pDevice, L"2D_Sprites\\Aru_down_8peaces.bmp", 512, 64, 8, D3DCOLOR_XRGB(0, 170, 255));
	m_sprite[LEFT].Create_Sprite(_pDevice, L"2D_Sprites\\Aru_left_8peaces.bmp", 512, 64, 8, D3DCOLOR_XRGB(0, 170, 255));
	m_sprite[ATTACK].Create_Sprite(_pDevice, L"2D_Sprites\\Aru_attack_6peaces.bmp", 480, 48, 6, D3DCOLOR_XRGB(0, 170, 255));

	// 캐릭터 좌우 움직임 먼지 파티클
	m_Run_Particle_Sprite.Create_Sprite(_pDevice, L"2D_Sprites\\Run_Particle.png", 80, 16, 5, D3DCOLOR_XRGB(65, 188, 222));

	m_FireBall.Create_Sprite(_pDevice, L"2D_Sprites\\fireball.png", 576, 96, 6, NULL);
	m_FireBall_Hit.Create_Sprite(_pDevice, L"2D_Sprites\\fireball_hit.png", 576, 64, 9, NULL);
}

void CAru::Update(LPDIRECT3DDEVICE9 _pDevice)
{
	isCrash_Tile();
	isCrash_Enemy();
	isCrash_Potion();

	Check_Collision_is_Possible(); // 점프 시에 타일과 충돌 가능상태인지 확인
	
	if (m_bActive_Collision == true)
	{
		KeyInput(_pDevice);

		Jump();
	}
	Gravity();

	Attack_Cooltime();
	Skill_Update();
	Skill_Destory();

	Set_Animation();

	m_sprite[m_iAnimate_Num].Animation_Frame();

	if (m_iAnimate_Num == 4 && m_sprite[m_iAnimate_Num].Get_Last_Frame() == true)
	{
		m_bAttacking = false;
	}

	if (m_b_isRunning && !m_bJump)
	{
		m_Run_Particle_Sprite.Animation_Frame();
	}

	//Set_Collider(m_sprite[m_iAnimate_Num].Get_Sprite_Width() - 20.0f, m_sprite[m_iAnimate_Num].Get_Sprite_Height(), true, RECT{0, 20, 0, 0});
	Set_Collider(44.0f, 64.0f, true, RECT{ 0, 20, 0, 0 });
}

void CAru::Render()
{
	m_sprite[m_iAnimate_Num].DrawBitmap(&m_vPos, 0xFFFFFFFF, Check_Reverse());
	
	if (m_b_isRunning && !m_bJump)
	{
		if (!Check_Reverse())
			m_Run_Particle_Sprite.DrawBitmap(&(m_vPos + D3DXVECTOR3(28.0f, 28.0f, 0.0f)), 0xFFFFFFFF, Check_Reverse());
		else if (Check_Reverse())
			m_Run_Particle_Sprite.DrawBitmap(&(m_vPos + D3DXVECTOR3(-28.0f, 28.0f, 0.0f)), 0xFFFFFFFF, Check_Reverse());
	}

	Skill_Render();

	//Draw_RectLine();
}

void CAru::Clean()
{
	for (int i = 0; i < 5; ++i)
	{
		m_sprite[i].CleanUp();
	}

	m_Run_Particle_Sprite.CleanUp();
	Skill_Clean();
	m_FireBall.CleanUp();
	m_FireBall_Hit.CleanUp();
}

VOID CAru::KeyInput(LPDIRECT3DDEVICE9 _pDevice)
{
	// LEFT, RIGHT
	if (CInput::Get_Instance()->IsKeyPressed(DIK_LEFT) == true)
	{
		m_vPos.x -= m_fSpeed;
		m_vDirection.x = -1.0f;
		m_b_isRunning = true;
		currentDirection = -1;
	}
	else if (CInput::Get_Instance()->IsKeyPressed(DIK_RIGHT) == true)
	{
		m_vPos.x += m_fSpeed;
		m_vDirection.x = 1.0f;
		m_b_isRunning = true;
		currentDirection = 1;
	}

	// JUMP
	if (m_iJump < 2)
	{
		if (m_bJump_Input_Lock == false)
		{
			if (CInput::Get_Instance()->IsKeyPressed(DIK_SPACE) == true)
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
		}
	}
	
	if (CInput::Get_Instance()->IsKeyPressed(DIK_SPACE) == false)
	{
		m_bJump_Input_Lock = false;
	}

	// FireBall
	if (m_bAttack_Lock == false)
	{
		if (CInput::Get_Instance()->IsKeyPressed(DIK_Z) == true)
		{
			CFireBall* FireBall = new CFireBall();
			FireBall->Init(_pDevice, m_vPos, currentDirection);
			FireBall->Set_Sprite(m_FireBall, m_FireBall_Hit);
			m_FireBall_List.push_back(FireBall);

			m_fBefore_Clock = clock();

			m_bAttacking = true;
			m_bAttack_Lock = true;
		}		
	}
	

	if (CInput::Get_Instance()->IsKeyPressed(DIK_LEFT) == false &&
		CInput::Get_Instance()->IsKeyPressed(DIK_RIGHT) == false &&
		CInput::Get_Instance()->IsKeyPressed(DIK_UP) == false &&
		CInput::Get_Instance()->IsKeyPressed(DIK_DOWN) == false)
	{
		m_vDirection = { 0.0f, 0.0f };
		m_b_isRunning = false;
		m_Run_Particle_Sprite.Reset_Sprite();
	}
}