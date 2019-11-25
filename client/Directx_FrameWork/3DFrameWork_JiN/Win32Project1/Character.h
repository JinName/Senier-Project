#pragma once

#include "Header.h"
#include "DirectInput.h"
#include "Entity.h"

class CCharacter : public CEntity
{
protected:

public:
	CCharacter();
	//CCharacter();
	~CCharacter();

	// 윈도우의 키보드 값을 리턴 받아서 움직임
	//VOID Move();

	void Init();
	void Update();
	void Render();
	void Clean();
};

