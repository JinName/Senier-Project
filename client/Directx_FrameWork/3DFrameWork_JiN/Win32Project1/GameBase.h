#pragma once

#include "Header.h"
#include "D3DSetup.h"
//#include "Vertex.h"
//#include "Aru.h"

enum { TITLE, STAGE, GAMEOVER, GAMECLEAR };

class CGameBase : public CD3DSetup
{
protected:
	Position m_pos;

	bool m_bGameStart;
	bool m_bGameOver;
	bool m_bGameClear;
	bool m_bRestart;
	bool m_bExit;

	int m_iGameScene_Num;

protected:
	// 위치 셋팅 - 월드 좌표 변환
	void SetPosition(float _fx, float _fy, float _fz);

public:	
	CGameBase();
	~CGameBase() {};
	
	int Get_GameScene_Num() { return m_iGameScene_Num; }
	bool Get_Change_GameStart() { return m_bGameStart; }
	bool Get_Change_GameOver() { return m_bGameOver; }
	bool Get_Change_Clear() { return m_bGameClear; }
	bool Get_Change_Restart() { return m_bRestart; }
	bool Get_Exit() { return m_bExit; }
	
	// 부모의 함수
	virtual void OnInit(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnUpdate(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnRender(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnCleanup(LPDIRECT3DDEVICE9 _pDevice);
};
