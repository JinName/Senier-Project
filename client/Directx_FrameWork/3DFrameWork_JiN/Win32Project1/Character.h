#pragma once

#include "Header.h"
#include "DirectInput.h"
#include "Entity.h"

class CCharacter : public CEntity
{
protected:
	bool m_bIsPlayer;

public:
	CCharacter() : m_bIsPlayer(false) {}
	~CCharacter() {}

	// �������� Ű���� ���� ���� �޾Ƽ� ������
	//VOID Move();

	void SetIsPlayer(bool _bIsPlayer) { m_bIsPlayer = _bIsPlayer; }

	void Init();
	void Update();
	void Render();
	void Clean();
};

