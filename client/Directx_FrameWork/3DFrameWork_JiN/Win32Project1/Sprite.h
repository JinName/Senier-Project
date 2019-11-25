#pragma once
#include "Header.h"
//#include "WinConfigure.h"
#include "Texture.h"

class CSprite : public CTexture
{
private:
	LPD3DXSPRITE m_pSprite;

	int m_iWidth;
	int m_iHeight;
	int m_iSprite_Count;

	// 스프라이트 한칸의 너비, 높이
	int m_rectWidth;
	int m_rectHeight;

	// 스프라이트 재생 속도
	DWORD m_dwSpriteTime;
	DWORD m_dwOldTime;

	// 스프라이트의 중앙 값
	D3DXVECTOR3 m_vCenter;
	// 스프라이트를 중앙값 기준으로 그릴것인지 on / off
	// 기본값 true
	bool m_bCenter_Rect;

	// 스프라이트 용
	RECT m_SpriteRect;

	// 스프라이트 애니매이션 on / off
	// 기본값 true
	bool m_bAnimation_Switch;

	// 마지막 프레임이면 true 리턴
	bool m_bLast_Frame;

public:
	CSprite();
	~CSprite();

	// 마지막 프레임인지 반환
	bool Get_Last_Frame() { return m_bLast_Frame; }

	// 스프라이트 좌우 크기 반환
	int Get_Sprite_Width() { return m_rectWidth; }
	int Get_Sprite_Height() { return m_rectHeight; }

	// 중앙값 계산 스위치
	bool Get_Center_Rect() { return m_bCenter_Rect; }
	void Set_Center_Rect(bool _bCenter_Rect) { m_bCenter_Rect = _bCenter_Rect; }

	// 스프라이트 스위치
	bool Get_Animation_Switch() { return m_bAnimation_Switch; }
	void Set_Animation_Switch(bool _bAnimation_Switch) { m_bAnimation_Switch = _bAnimation_Switch; }

	// 스프라이트 생성 (디바이스, 텍스쳐 경로, 알파값처리용 컬러키, )
	HRESULT Create_Sprite(LPDIRECT3DDEVICE9 _pDevice, std::wstring _filePath, int _width, int _height, int _spriteCount, D3DCOLOR mask);

	void DrawBitmap(D3DXVECTOR3* pos, D3DCOLOR mask, bool reverse);

	// 애니매이션
	void Animation_Frame();
	// 원하는 스프라이트 선택 : 보통 스프라이트 스위치 off 상태에서 쓸 것
	void Select_Sprite(int _Sprite_Num);

	void Reset_Sprite();

	void CleanUp();
};

