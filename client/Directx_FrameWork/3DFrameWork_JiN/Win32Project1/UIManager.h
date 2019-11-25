#pragma once
#include "Header.h"
#include "Health.h"

class CUIManager
{
private:
	// HP
	CHealth m_Health;
public:
	CUIManager();
	~CUIManager();

	void Init(LPDIRECT3DDEVICE9 _pDevice, int _Player_HP);
	void Update(int _Player_HP);
	void Render();
	void Clean();
};

