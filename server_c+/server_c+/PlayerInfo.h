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
	// ĳ���� ��ġ
	VECTOR3 m_vPos;

	float m_fBefore_Clock;
	bool m_bAttack_Lock;
	float m_fAttack_Cooltime;

	// ĳ���� �ӵ�
	float m_fSpeed;

	// ĳ���� ����
	VECTOR2 m_vDirection;

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
};

