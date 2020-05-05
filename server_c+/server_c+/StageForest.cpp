#include "StageForest.h"

void StageForest::Init()
{
	CreateTiles();
}

void StageForest::CreateTiles()
{
	m_Tile = new Tile[8];

	m_Tile[0].Init(VECTOR3(-15.0f, 640.0f, 0.0f), 433, 84, false, RECT{ 8, 23, -8, 0 });
	m_Tile[1].Init(VECTOR3(470.0f, 540.0f, 0.0f), 168, 76, false, RECT{ 8, 23, -8, -5 });
	m_Tile[2].Init(VECTOR3(720.0f, 590.0f, 0.0f), 433, 84, false, RECT{ 8, 23, -8, 0 });
	m_Tile[3].Init(VECTOR3(165.0f, 470.0f, 0.0f), 267, 79, false, RECT{ 8, 23, -8, -5 });
	m_Tile[4].Init(VECTOR3(560.0f, 370.0f, 0.0f), 433, 84, false, RECT{ 8, 23, -8, 0 });
	m_Tile[5].Init(VECTOR3(650.0f, 100.0f, 0.0f), 267, 79, false, RECT{ 8, 23, -8, -5 });
	m_Tile[6].Init(VECTOR3(500.0f, 250.0f, 0.0f), 168, 76, false, RECT{ 8, 23, -8, -5 });
	m_Tile[7].Init(VECTOR3(0.0f, 160.0f, 0.0f), 433, 84, false, RECT{ 8, 23, -8, 0 });

	m_iTileCount = 8;
}