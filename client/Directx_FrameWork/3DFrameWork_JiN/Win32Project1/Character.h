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

	// 윈도우의 키보드 값을 리턴 받아서 움직임
	//VOID Move();

	void SetIsPlayer(bool _bIsPlayer) { m_bIsPlayer = _bIsPlayer; }

	void Init();
	void Update();
	void Render();
	void Clean();
};

