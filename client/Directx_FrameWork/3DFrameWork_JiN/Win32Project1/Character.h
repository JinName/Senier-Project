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

	// �������� Ű���� ���� ���� �޾Ƽ� ������
	//VOID Move();

	void Init();
	void Update();
	void Render();
	void Clean();
};

