#pragma once
#include "Header.h"
#include "Character.h"
#include "Sprite.h"
#include "FireBall.h"

enum{ STAND, UP, DOWN, LEFT, ATTACK };

class CAru : public CCharacter
{
private:
	// ���ĵ�, �� �� ��(��), ����
	CSprite m_sprite[5];
	CSprite m_Run_Particle_Sprite;

	// FireBall Sprite �̸� �ε��س��� �ؽ�ó���� �Ѱ��ִ� ���
	CSprite m_FireBall;
	CSprite m_FireBall_Hit;

	list<CFireBall*> m_FireBall_List;
	float m_fBefore_Clock;
	bool m_bAttack_Lock;
	float m_fAttack_Cooltime;

	// ĳ���� �ӵ�
	float m_fSpeed;

	// ĳ���� ����
	D3DXVECTOR2 m_vDirection;

	int m_iJump; // 2�� ������
	bool m_bJump_Input_Lock;
	// �ൿ ����
	bool m_bAttacking;
	bool m_bJump;
	bool m_b_isRunning;
	float m_fJump_Power;
	float m_fGravity_Accel;
	// ���� �뵵
	// ���� ������ �����϶� true (�ٴڿ� �پ�������)
	bool m_bJump_is_Possible;
	// ������ �浹 ���ɻ���
	bool m_bCollision_is_Possible;
	// ������ �� ������ ����
	float m_fOld_Pos_y;
	bool m_bOld_Check;

	// Ÿ�� �浹�ڽ� �ؿ��� �ö� ���
	// �浹 �غ� ���� ��� true;
	// �񱳴���� �浹�ڽ����� Ȯ���ϰ� ���� �ö����� true��
	bool m_bReady_to_Collision;

	// ������ ������ ���� ( �¿� ��������Ʈ ������ )
	int currentDirection;

	// �ִϸ��̼� ��ȣ
	int m_iAnimate_Num;
	// �ִϸ��̼� ���� ��������Ʈ ũ�Ⱑ �ٸ��Ƿ� RECT �缳�� �ʿ�

	// �߷°��� �ð�
	DWORD dwOldtime;
	float m_fCharacter_mass; // ĳ���� ����
	// �ӷ�
	float velocity;

	// �浹 �� ĳ���� ������ ����
	bool isVertical; // ���� �浹
	bool isHorizontal; // ���� �浹
	bool isHit; // �ǰ�

	int m_iHP; // ü��		
	bool m_bAfter_Collision_Setting; // �浹 �� �÷��̾� ���� ����
	bool m_bActive_Collision; // �ǰ� �� ��������
	float m_fCollision_Power; // �浹�� �з����� ��

	// ���ǰ� �浹��
	bool m_bHP_isFull; // HP FULL �̸� true : ���� ������
	bool m_bPotion_Collision; // ���ǰ� �浹�� true

public:
	CAru();
	~CAru();

	// HP Ǯ�϶� true
	bool Get_HP_isFull();
	// ���� �浹�� true
	void Set_Potion_Collision(bool _bPotion_Collision) { m_bPotion_Collision = _bPotion_Collision; }

	// �ǰݽ� on/off : ���Ϳ� �浹 ���� �������� ��ȯ
	bool Get_Active_Collision() { return m_bActive_Collision; }
	void Set_Active_Collision(bool _bActive_Collision) { m_bActive_Collision = _bActive_Collision; }

	// hp ��ȯ
	int Get_HP() { return m_iHP; }

	// ���̾ ����Ʈ ��ȯ - �浹ó��
	list<CFireBall*> Get_FireBall_List() { return m_FireBall_List; }

	// ������ on/off Ÿ�ϰ� �浹 ���� �������� ��ȯ
	bool Get_Collision_is_Possible() { return m_bCollision_is_Possible; }
	// �浹 �� ���� ����
	void Set_isVertical(bool _isVertical) { isVertical = _isVertical; }
	void Set_isHorizontal(bool _isHorizontal) { isHorizontal = _isHorizontal; }
	void Set_isHit(bool _isHit) { isHit = _isHit; }

	// ���⿡ ���� �ִϸ��̼� ��ȣ ���� �Լ�
	void Set_Animation();
	// ĳ���� �ֱ� ���¿� ���� DrawBitmap Reverse ����
	bool Check_Reverse();

	// ĳ���� �߷�
	void Gravity();
	// ĳ���� ����
	void Jump();

	// ĳ���Ͱ� Ÿ�ϰ� �浹 �� ������
	void isCrash_Tile();
	// ���� �ε�������
	void isCrash_Enemy();
	// ���ǰ� �浹 ��
	void isCrash_Potion();

	// �浹 ������ ��������
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

