//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: text_box.cpp
// Description	: Simple UI textbox to get the users input 
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//


// Local Includes
#include "services.h"

// This Include 
#include "text_box.h"

// Implementation
CTextBox::CTextBox() :
	CPointSprite(), m_sText(), m_pFont( nullptr ), m_bSelected( false )
{
	m_HitBox.top	= 0;
	m_HitBox.left	= 0;
	m_HitBox.bottom	= 0;
	m_HitBox.right	= 0;

	m_TextBox = m_HitBox;
}

CTextBox::CTextBox( const D3DXVECTOR2& _vPosition, const D3DXVECTOR2& _vScale, 
	const D3DXVECTOR2& _vTextOffset, ID3DX10Font* _pFont ) :
	CPointSprite( _vPosition, _vScale, 0.0f, g_TextBoxColorDefault ),
	m_sText(), m_bSelected( false ), m_pFont( _pFont )
{
	m_HitBox.top	= static_cast< long >( _vPosition.y + ( _vScale.y / 2.0f ) );
	m_HitBox.left	= static_cast< long >( _vPosition.x - ( _vScale.x / 2.0f ) );
	m_HitBox.bottom	= static_cast< long >( _vPosition.y - ( _vScale.y / 2.0f ) );
	m_HitBox.right	= static_cast< long >( _vPosition.x + ( _vScale.x / 2.0f ) );

	m_TextBox.top		= static_cast< long >( _vTextOffset.y - m_HitBox.top );
	m_TextBox.left		= static_cast< long >( _vTextOffset.x + m_HitBox.left );
	m_TextBox.bottom	= static_cast< long >( _vTextOffset.y - m_HitBox.bottom );
	m_TextBox.right		= static_cast< long >( _vTextOffset.x + m_HitBox.right );
		
	void* vpKeyboard;
	CServices::GetService( SERVICETYPE_KEYBOARD, vpKeyboard );
	m_pKeyboard = static_cast< CKeyboardInput* >( vpKeyboard );
}

CTextBox::~CTextBox()
{}

std::string CTextBox::GetText() const
{
	return ( m_sText );
}

void CTextBox::SetText( const std::string _sText )
{
	m_sText = _sText;
}

bool CTextBox::GetSelected() const
{
	return ( m_bSelected );
}

void CTextBox::SetSelected( const bool _bSelected )
{
	m_bSelected = _bSelected;

	m_vColor = m_bSelected ? g_TextBoxColorSelected : g_TextBoxColorDefault;
}

void CTextBox::Update()
{
	if( m_bSelected )
	{
		// Ensure that entered field doesn't go over max
		if( m_sText.length() < g_TextBoxMaxCharacters )
		{
			for( int i = 0; i < 26; ++i )
			{
				if( m_pKeyboard->KeyPressed( 65 + i ) )
				{
					m_sText += 0x41 + i;
				}
			}
		}
	
		if( m_sText.length() > 0 )
		{
			if( m_pKeyboard->KeyPressed( VK_BACK ) )
			{
				m_sText.pop_back();
			}
		}
	}
}

void CTextBox::OutputText()
{
	bool bText = m_sText.length() > 0;
	m_pFont->DrawTextA( 0, bText ? m_sText.c_str() : g_TextBoxTextDefault.c_str(), -1, &m_TextBox , DT_SINGLELINE | DT_CENTER | DT_VCENTER , WHITE );
}

bool CTextBox::InHitBox( const D3DXVECTOR2 _vPosition )
{
	return ( _vPosition.x >= m_HitBox.left && _vPosition.x <= m_HitBox.right && 
			_vPosition.y <= m_HitBox.top && _vPosition.y >= m_HitBox.bottom );
}