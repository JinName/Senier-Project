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
	// 충돌박스들을 리스트로 가지고 있음
	// ex) Character vs TileList 타일들과 충돌체크
	C2DBoxCollider m_boxCollider;
public:
	CCollisionManager();
	~CCollisionManager();

	void Init();
	//void Update(list<RECT> &_Rect_List, CAru &_aru);
	
	// 캐릭터 - 타일 충돌 : 캐릭터가 타일 위를 움직임
	void Charater_Tile_Check(CTile* _Tile_Array, int _Tile_Array_Len, CAru &_aru);

	// 캐릭터 공격 - 몬스터 충돌 : 몬스터 데미지 입음
	void CharAttack_Monster_Check(list<CFireBall*> &_FireBall_List, list<CMonster> &_Monster_List);

	// 캐릭터 - 몬스터 충돌 : 캐릭터 데미지 입음
	void Charater_Monster_Check(CAru &_aru, list<CMonster> &_Monster_List);

	// 캐릭터 - 포션 충돌 : 캐릭터 체력 회복
	void Charater_Potion_Check(CAru &_aru, list<CPotion> &_Potion_List);
};

