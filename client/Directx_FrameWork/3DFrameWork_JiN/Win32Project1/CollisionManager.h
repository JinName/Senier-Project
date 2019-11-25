#pragma once
#include "Header.h"
#include "2DBoxCollider.h"
#include "Entity.h"
#include "Tile.h"
#include "Aru.h"
#include "Monster.h"
#include "Potion.h"

class CCollisionManager
{
private:
	// �浹�ڽ����� ����Ʈ�� ������ ����
	// ex) Character vs TileList Ÿ�ϵ�� �浹üũ
	C2DBoxCollider m_boxCollider;
public:
	CCollisionManager();
	~CCollisionManager();

	void Init();
	//void Update(list<RECT> &_Rect_List, CAru &_aru);
	
	// ĳ���� - Ÿ�� �浹 : ĳ���Ͱ� Ÿ�� ���� ������
	void Charater_Tile_Check(CTile* _Tile_Array, int _Tile_Array_Len, CAru &_aru);

	// ĳ���� ���� - ���� �浹 : ���� ������ ����
	void CharAttack_Monster_Check(list<CFireBall*> &_FireBall_List, list<CMonster> &_Monster_List);

	// ĳ���� - ���� �浹 : ĳ���� ������ ����
	void Charater_Monster_Check(CAru &_aru, list<CMonster> &_Monster_List);

	// ĳ���� - ���� �浹 : ĳ���� ü�� ȸ��
	void Charater_Potion_Check(CAru &_aru, list<CPotion> &_Potion_List);
};

