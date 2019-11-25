#pragma once
#include "Header.h"
#include "Sprite.h"
#include "Entity.h"

// �����ڿ��� ĳ������ ��ġ�� �޾� �߻���ġ, ������ ����
class CFireBall : public CEntity
{
private:
	// ��������Ʈ
	CSprite m_FireBall;
	CSprite m_FireBall_Hit;

	float m_fDamage; // ������
	float m_fSpeed; // ����ü �ӵ�
	int m_iDirection; // �߻� ���� ( ĳ���� ���⿡ ���� )

	bool m_b_is_Collision; // ���� �浹���� ��� true
	bool m_bHit_Ani; // ����������� ������ �ִϸ��̼�
	bool m_bCollision_is_Possible; // ������ �ִϸ��̼��� ���� �浹 false

	bool m_bDestroy; // true �� �Ҹ�

public:
	CFireBall();
	~CFireBall();

	bool Get_Collision_is_Possible() { return m_bCollision_is_Possible; }

	bool Get_isCollision() { return m_b_is_Collision; }
	void Set_isCollision(bool _isCollision) { m_b_is_Collision = _isCollision; }

	bool Get_isDestroy() { return m_bDestroy; }
	// ���̾ �߻� ��ġ ���� (ĳ���� ��)
	// ���� entity Ŭ������ set_position �Լ� �̿�

	void FireBall_Move();
	bool FireBall_Destroy_Check();

	bool Check_Reverse();

	// �̸� �ε��س��� ��������Ʈ ����
	void Set_Sprite(CSprite &_normalSprite, CSprite &_hitSprite);
	void Init(LPDIRECT3DDEVICE9 _pDevice, D3DXVECTOR3 _vPos, int _iDirection);
	void Update(); // ������Ʈ �Լ��� ĳ���� ���¸� �޾ƿ��Բ�, �߻� ��ġ, ���� �� ����
	void Render();
	void Clean();
};

