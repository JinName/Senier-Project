#pragma once

#include "Header.h"
/*
// A structure for our custom vertex type
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
};
*/

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

class CVertex
{
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;

	CUSTOMVERTEX cv;

public:
	CVertex();
	//CVertex(float _x, float _y, float _z, float _rhw, DWORD _color);
	~CVertex();

	// vertex 생성
	HRESULT InitVB(LPDIRECT3DDEVICE9 _d3dDevice);

	// m_pVB 반환
	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer() { return m_pVB; }

	//void SetVertex();

	// m_pVB 해제
	void Release();

};

