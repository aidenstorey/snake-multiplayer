//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: static_text.cpp
// Description	: Simple text ouput to ease formatting of UI
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

// This Include
#include "static_text.h"

// Implementation
CStaticText::CStaticText() :
	CPointSprite(), m_sText(), m_pFont( nullptr )
{
	m_TextBox.top		= 0;
	m_TextBox.left		= 0;
	m_TextBox.bottom	= 0;
	m_TextBox.right		= 0;
}

CStaticText::CStaticText( const D3DXVECTOR2& _vPosition, const D3DXVECTOR2& _vScale, const std::string _sText, 
	const D3DXVECTOR2& _vTextOffset, ID3DX10Font* _pFont, D3DXCOLOR _vBackgroundColor ) :
	CPointSprite( _vPosition, _vScale, 0.0f, _vBackgroundColor ),
	m_sText( _sText ), m_pFont( _pFont )
{
	m_TextBox.top		= static_cast< long >( _vTextOffset.y - ( _vPosition.y + ( _vScale.y / 2.0f ) ) );
	m_TextBox.left		= static_cast< long >( _vTextOffset.x + ( _vPosition.x - ( _vScale.x / 2.0f ) ) );
	m_TextBox.bottom	= static_cast< long >( _vTextOffset.y - ( _vPosition.y - ( _vScale.y / 2.0f ) ) );
	m_TextBox.right		= static_cast< long >( _vTextOffset.x + ( _vPosition.x + ( _vScale.x / 2.0f ) ) );
}

CStaticText::~CStaticText()
{}

std::string CStaticText::GetButtonText() const
{
	return ( m_sText );
}

void CStaticText::SetButtonText( const std::string _sText )
{
	m_sText = _sText;
}

void CStaticText::OutputText()
{
	m_pFont->DrawTextA( 0, m_sText.c_str(), -1, &m_TextBox, DT_CENTER, WHITE );
}

bool CStaticText::InHitBox( const D3DXVECTOR2 _vPosition )
{
	return ( false );
}