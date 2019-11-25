#pragma once
#include <assert.h>
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 )
#include <strsafe.h>
#pragma warning( default : 4996 )

#include <list>
#include <stdio.h>
#include <iostream>
#include <ctime>
using namespace std;


#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }
#define SAFE_DELETE(a) if( (a) != NULL ) delete (a); (a) = NULL;


//#define POINT D3DLIGHT_POINT
#define DIRECT D3DLIGHT_DIRECTIONAL
//#define SPOT D3DLIGHT_SPOT

struct Position {
	// 캐릭터 위치
	float m_fx;
	float m_fy;
	float m_fz;

	Position() : m_fx(0.0f), m_fy(0.0f), m_fz(0.0f)
	{	}
	~Position(){}
};

struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
};
