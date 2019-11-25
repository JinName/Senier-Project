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

	// ��������Ʈ ��ĭ�� �ʺ�, ����
	int m_rectWidth;
	int m_rectHeight;

	// ��������Ʈ ��� �ӵ�
	DWORD m_dwSpriteTime;
	DWORD m_dwOldTime;

	// ��������Ʈ�� �߾� ��
	D3DXVECTOR3 m_vCenter;
	// ��������Ʈ�� �߾Ӱ� �������� �׸������� on / off
	// �⺻�� true
	bool m_bCenter_Rect;

	// ��������Ʈ ��
	RECT m_SpriteRect;

	// ��������Ʈ �ִϸ��̼� on / off
	// �⺻�� true
	bool m_bAnimation_Switch;

	// ������ �������̸� true ����
	bool m_bLast_Frame;

public:
	CSprite();
	~CSprite();

	// ������ ���������� ��ȯ
	bool Get_Last_Frame() { return m_bLast_Frame; }

	// ��������Ʈ �¿� ũ�� ��ȯ
	int Get_Sprite_Width() { return m_rectWidth; }
	int Get_Sprite_Height() { return m_rectHeight; }

	// �߾Ӱ� ��� ����ġ
	bool Get_Center_Rect() { return m_bCenter_Rect; }
	void Set_Center_Rect(bool _bCenter_Rect) { m_bCenter_Rect = _bCenter_Rect; }

	// ��������Ʈ ����ġ
	bool Get_Animation_Switch() { return m_bAnimation_Switch; }
	void Set_Animation_Switch(bool _bAnimation_Switch) { m_bAnimation_Switch = _bAnimation_Switch; }

	// ��������Ʈ ���� (����̽�, �ؽ��� ���, ���İ�ó���� �÷�Ű, )
	HRESULT Create_Sprite(LPDIRECT3DDEVICE9 _pDevice, std::wstring _filePath, int _width, int _height, int _spriteCount, D3DCOLOR mask);

	void DrawBitmap(D3DXVECTOR3* pos, D3DCOLOR mask, bool reverse);

	// �ִϸ��̼�
	void Animation_Frame();
	// ���ϴ� ��������Ʈ ���� : ���� ��������Ʈ ����ġ off ���¿��� �� ��
	void Select_Sprite(int _Sprite_Num);

	void Reset_Sprite();

	void CleanUp();
};

