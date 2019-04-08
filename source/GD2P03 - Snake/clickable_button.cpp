//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: clickable_button.cpp
// Description	: Emulates a user interface button with a command
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

// Local Includes
#include "game_manager.h"

// This Include 
#include "clickable_button.h"

// Implementation
CButton::CButton() :
	CPointSprite(), m_sText(), m_pInvoker( nullptr ), m_pPressedFunc( nullptr ), 
	m_pFont( nullptr ), m_bEnabled( false )
{
	// Initialise the hit box size
	m_HitBox.top	= 0;
	m_HitBox.left	= 0;
	m_HitBox.bottom	= 0;
	m_HitBox.right	= 0;

	m_TextBox = m_HitBox;
}

CButton::CButton( const D3DXVECTOR2& _vPosition, const D3DXVECTOR2& _vScale, const std::string _sText, 
	const D3DXVECTOR2& _vTextOffset, ID3DX10Font* _pFont, CGameManager* _pInvoker, void ( CGameManager::*_pPressedFunc ) (), bool _bEnabled ) :
	CPointSprite( _vPosition, _vScale, 0.0f, g_ButtonColorDefault ), 
	m_sText( _sText ), m_pInvoker( _pInvoker ), m_pPressedFunc( _pPressedFunc ), 
	m_pFont( _pFont ), m_bEnabled( _bEnabled )
{
	// Sets the hitbox size based on input
	m_HitBox.top	= static_cast< long >( _vPosition.y + ( _vScale.y / 2.0f ) );
	m_HitBox.left	= static_cast< long >( _vPosition.x - ( _vScale.x / 2.0f ) );
	m_HitBox.bottom	= static_cast< long >( _vPosition.y - ( _vScale.y / 2.0f ) );
	m_HitBox.right	= static_cast< long >( _vPosition.x + ( _vScale.x / 2.0f ) );

	// Sets the text box size based on input
	m_TextBox.top		= static_cast< long >( _vTextOffset.y - m_HitBox.top );
	m_TextBox.left		= static_cast< long >( _vTextOffset.x + m_HitBox.left );
	m_TextBox.bottom	= static_cast< long >( _vTextOffset.y - m_HitBox.bottom );
	m_TextBox.right		= static_cast< long >( _vTextOffset.x + m_HitBox.right );
}

CButton::~CButton()
{}

std::string CButton::GetButtonText() const
{
	return ( m_sText );
}

void CButton::SetButtonText( std::string _sText )
{
	m_sText = _sText;
}

bool CButton::GetEnabled() const
{
	return ( m_bEnabled );
}

void CButton::SetEnabled( const bool _bEnable )
{
	m_bEnabled = _bEnable;
}

void CButton::SetPressedFunction( CGameManager* _pInvoker, void ( CGameManager::*_pPressedFunc ) () )
{
	m_pInvoker		= _pInvoker;
	m_pPressedFunc	= _pPressedFunc;
}

bool CButton::IsBeingPressed()
{
	return ( m_bBeingPressed );
}

void CButton::OutputText()
{
	// Draws the button text to the given box
	m_pFont->DrawTextA( 0, m_sText.c_str(), -1, &m_TextBox, DT_SINGLELINE | DT_CENTER | DT_VCENTER , m_bEnabled ? WHITE : D3DXCOLOR( 0.5f, 0.5f, 0.5f, 1.0f ) );
}

void CButton::ExecuteFunction()
{
	// If function is assigned to the button it will run
	if( m_pPressedFunc != nullptr )
	{
		( m_pInvoker->*m_pPressedFunc ) ();
		m_bBeingPressed = true;
	}
}

bool CButton::InHitBox( const D3DXVECTOR2 _vPosition )
{
	// Initialise default return
	bool bReturn = false;

	if( m_bEnabled )
	{	
		// Set color to default button color
		m_vColor = g_ButtonColorDefault;

		// Check if position is in hit box
		if( _vPosition.x >= m_HitBox.left && _vPosition.x <= m_HitBox.right && 
			_vPosition.y <= m_HitBox.top && _vPosition.y >= m_HitBox.bottom )
		{
			// Set return to true
			bReturn = true;

			// Set color to hover button color
			m_vColor = g_ButtonColorHover;
		}
	}
	else
	{
		// Set color to disabled button color
		m_vColor = g_ButtonColorDisabled;
	}

	return( bReturn );
}