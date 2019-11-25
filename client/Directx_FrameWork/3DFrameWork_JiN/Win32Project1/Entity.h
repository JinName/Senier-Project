#pragma once
#include "Header.h"

class CEntity
{
protected:
	// 위치 Position
	D3DXVECTOR3 m_vPos;

	// 충돌 박스
	RECT m_rt;

	// 충돌 검사용 선 그리기 변수
	LPD3DXLINE m_pLine;
	D3DXVECTOR2 vList[5];
	
public:
	CEntity();
	~CEntity();

	// 매니저에서 접근용 함수
	D3DXVECTOR3 Get_Position() { return m_vPos; }

	// 충돌 박스 반환 함수
	RECT Get_Collider() { return m_rt; }

	// 충돌 박스 범위 그리기
	void Line_Init(LPDIRECT3DDEVICE9 _pDevice);
	void Draw_RectLine();

	// 충돌 박스 셋팅
	void Set_Collider(float _width, float _height, bool _centered_rect);
	void Set_Collider(float _width, float _height, bool _centered_rect, RECT _detail_rect);

	// 기본 포지션 변환 함수
	void Set_Position(float _x, float _y, float _z) {
		m_vPos.x = _x;
		m_vPos.y = _y;
		m_vPos.z = _z;
	}
};

