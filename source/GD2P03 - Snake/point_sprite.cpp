//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: point_sprite.cpp
// Description	: Basic drawing object to help reduce cpu computation
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

// This include
#include "point_sprite.h"

// Implementation
CPointSprite::CPointSprite() :
	m_vPosition( 0.0f, 0.0f ), m_vScale( 0.0f, 0.0f ),
	m_fRotation( 0.0f ), m_vColor( 0.0f, 0.0f, 0.0f, 1.0f )
{}

CPointSprite::CPointSprite( const D3DXVECTOR2& _vPosition, const D3DXVECTOR2& _vScale, 
		const float& _fRotation, const D3DXCOLOR& _vColor  ) :
	m_vPosition( _vPosition ), m_vScale( _vScale ), 
	m_fRotation( _fRotation ), m_vColor( _vColor )

{}
	
CPointSprite::~CPointSprite()
{}

D3DXVECTOR2 CPointSprite::GetPosition() const
{
	return ( m_vPosition );
}

void CPointSprite::SetPosition( const D3DXVECTOR2& _vPosition )
{
	m_vPosition = _vPosition;
}

D3DXVECTOR2 CPointSprite::GetScale() const
{
	return ( m_vScale );
}

void CPointSprite::SetScale( const D3DXVECTOR2& _vScale )
{
	m_vScale = _vScale;
}

float CPointSprite::GetRotation() const
{
	return ( m_fRotation );
}

void CPointSprite::SetRotation( const float _fRotation )
{
	m_fRotation = _fRotation;
}

D3DXCOLOR CPointSprite::GetColor() const
{
	return ( m_vColor );
}

void CPointSprite::SetColor( const D3DXCOLOR _vColor )
{
	m_vColor = _vColor;
}

bool CPointSprite::InHitBox( const D3DXVECTOR2 _vPosition )
{
	return ( false );
}