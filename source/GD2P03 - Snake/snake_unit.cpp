//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: snake_unit.cpp
// Description	: Abstracted unit for simple displaying on the board
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

// This Include 
#include "snake_unit.h"

// Implementation
CSnakeUnit::CSnakeUnit() :
	CPointSprite()
{}

CSnakeUnit::CSnakeUnit( const D3DXVECTOR2& _vPosition, const D3DXVECTOR2& _vScale, const D3DXCOLOR& _vColor ):
	CPointSprite( _vPosition, _vScale, 0.0f, _vColor )
{}

CSnakeUnit::~CSnakeUnit()
{}

bool CSnakeUnit::InHitBox( const D3DXVECTOR2 _vPosition )
{
	return ( false );
}