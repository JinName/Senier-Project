#pragma once
#include "Header.h"
#include "Entity.h"
#include "Sprite.h"

class CMonster : public CEntity
{
private:
	CSprite m_Sprite;
	CSprite m_Monster_Hit; // ���� �ǰ� ��������Ʈ
	bool m_bHit_Ani; // �ǰ� �ִϸ��̼�

	// HP
	int m_iHP;

	// �¿� �ִ� �ּ� �̵� ����
	float m_fMax_x;
	float m_fMin_x;

	// �� �� ó�� �̵����� ����
	float m_iDirection;

	// �̵� �ӵ�
	float m_fSpeed;

	// �浹���� ��
	bool m_b_is_Collision;
	int m_iCollision_Direction; // �浹 ����
	float m_fCollision_Power; // �浹�� �з����� ��

	// HP �� ��������
	bool m_bDestroy;
public:
	CMonster();
	~CMonster();

	void Set_Collision_Direction(int _iCollision_Direction) { m_iCollision_Direction = _iCollision_Direction; }

	bool Get_Destroy() { return m_bDestroy; }

	bool Get_isCollision() { return m_b_is_Collision; }
	void Set_isCollision(bool _b_is_Collision) { m_b_is_Collision = _b_is_Collision; }

	// ���� �¿� ������
	void Monster_Move();
	// ���� �浹 ������
	void Monster_Collision_Move();

	// �浹��
	void Monster_Collision_Check();

	// ���� �Ҹ�
	void Monster_Destroy();

	void Init(LPDIRECT3DDEVICE9 _pDevice, D3DXVECTOR3 _vPos, float _fMove_Width);
	void Update();
	void Render();
	void Clean();
};

