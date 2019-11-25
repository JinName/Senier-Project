#pragma once
#include "Header.h"
#include "Entity.h"
#include "Sprite.h"

// �� ����� ���͸� �����ʰ� �׳� ���� ���� �������� �׸���
class CBackground :public CEntity
{
private:
	CSprite m_Sprite;
public:
	CBackground();
	~CBackground();

	void Init(LPDIRECT3DDEVICE9 _pDevice);
	void Update();
	void Render();
	void Clean();
};

