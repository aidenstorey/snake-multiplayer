//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: mouse_input.cpp
// Description	: Handles the state of the mouse at any given time
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

// This Include 
#include "mouse_input.h"

// Implemetnation
CMouseInput::CMouseInput() :
	m_hWnd( NULL ), m_iWidth( 0 ), m_iHeight( 0 )
{
	m_Position.x = 0;
	m_Position.y = 0;

	for( int i = 0; i < g_kiMouseMaxButtons; ++i )
	{
		m_bButtonCurr[ i ] = false;
		m_bButtonPrev[ i ] = false;
	}
}

CMouseInput::~CMouseInput()
{}

D3DXVECTOR2 CMouseInput::GetPosition() const
{
	return ( m_Position );
}

HWND CMouseInput::GetHWND() const
{
	return ( m_hWnd );
}

void CMouseInput::SetHWND( const HWND _hWnd )
{
	m_hWnd = _hWnd;
}

void CMouseInput::SetWindowSize( const int _kiWidth, const int _kiHeight )
{
	m_iWidth	= _kiWidth;
	m_iHeight	= _kiHeight;
}

bool CMouseInput::LeftPressed() const
{
	return( m_bButtonCurr[ 0 ] && !m_bButtonPrev[ 0 ] );
}

bool CMouseInput::LeftDown() const
{
	return( m_bButtonCurr[ 0 ] );
}

bool CMouseInput::LeftReleased() const
{
	return( !m_bButtonCurr[ 0 ] && m_bButtonPrev[ 0 ] );
}

bool CMouseInput::MiddlePressed() const
{
	return( m_bButtonCurr[ 1 ] && !m_bButtonPrev[ 1 ] );
}

bool CMouseInput::MiddleDown() const
{
	return( m_bButtonCurr[ 1 ] );
}

bool CMouseInput::MiddleReleased() const
{
	return( !m_bButtonCurr[ 1 ] && m_bButtonPrev[ 1 ] );
}

bool CMouseInput::RightPressed() const
{
	return( m_bButtonCurr[ 2 ] && !m_bButtonPrev[ 2 ] );
}

bool CMouseInput::RightDown() const
{
	return( m_bButtonCurr[ 2 ] );
}

bool CMouseInput::RightReleased() const
{
	return( !m_bButtonCurr[ 2 ] && m_bButtonPrev[ 2 ] );
}

void CMouseInput::ButtonDown( unsigned int _uiMsg )
{
	switch( _uiMsg )
	{
	case WM_LBUTTONDOWN:
		{
			m_bButtonPrev[ 0 ] = m_bButtonCurr[ 0 ];
			m_bButtonCurr[ 0 ] = true;
		}
		break;

	case WM_MBUTTONDOWN:
		{
			m_bButtonPrev[ 1 ] = m_bButtonCurr[ 1 ];
			m_bButtonCurr[ 1 ] = true;
		}
		break;

	case WM_RBUTTONDOWN:
		{
			m_bButtonPrev[ 2 ] = m_bButtonCurr[ 2 ];
			m_bButtonCurr[ 2 ] = true;
		}
		break;

	default: break;
	}
}

void CMouseInput::ButtonUp( unsigned int _uiMsg )
{
	switch( _uiMsg )
	{
	case WM_LBUTTONUP:
		{
			m_bButtonPrev[ 0 ] = m_bButtonCurr[ 0 ];
			m_bButtonCurr[ 0 ] = false;
		}
		break;

	case WM_MBUTTONUP:
		{
			m_bButtonPrev[ 1 ] = m_bButtonCurr[ 1 ];
			m_bButtonCurr[ 1 ] = false;
		}
		break;

	case WM_RBUTTONUP:
		{
			m_bButtonPrev[ 2 ] = m_bButtonCurr[ 2 ];
			m_bButtonCurr[ 2 ] = false;
		}
		break;

	default: break;
	}
}

void CMouseInput::Update()
{
	POINT pGet;
	GetCursorPos( &pGet );
	ScreenToClient( m_hWnd, &pGet ); 
	m_Position.x = static_cast< float >( pGet.x - m_iWidth / 2 );
	m_Position.y = static_cast< float >( ( pGet.y - m_iHeight / 2 ) * -1 );
}