#pragma once
#include "stdafx.h"
#include "Tile.h"

class StageBase
{
public:
	virtual void Init() = 0;
	virtual void CreateTiles() = 0;

	Tile	GetTile(int _index) { return m_Tile[_index]; }
	Tile*	GetTileArray() { return m_Tile; }
	int		GetTileCount() { return m_iTileCount; }

protected:
	Tile*	m_Tile;
	int		m_iTileCount;
};

