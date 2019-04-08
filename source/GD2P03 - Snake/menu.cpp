//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: menu.cpp
// Description	: Handles the drawing and management of UI elements for a displayable menu
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

// Local Includes
#include "services.h"

// This Include
#include "menu.h"

// Implementation
CMenu::CMenu() :
	m_pDevice( nullptr ), m_pVertexBuffer( nullptr ), 
	m_pMouse( nullptr ),m_pKeyboard( nullptr )
{}

CMenu::~CMenu()
{
	ReleaseCOM( m_pVertexBuffer );
	
	for( unsigned i = 0; i < m_vecButtons.size(); ++i )
	{
		delete m_vecButtons[ i ];
	}
	m_vecButtons.clear();

	for( unsigned i = 0; i < m_vecTextBox.size(); ++i )
	{
		delete m_vecTextBox[ i ];
	}
	m_vecTextBox.clear();

	for( unsigned i = 0; i < m_vecStaticText.size(); ++i )
	{
		delete m_vecStaticText[ i ];
	}
	m_vecStaticText.clear();

	// Deletion handled above
	m_vecVertices.clear();

	// Will be deleted else where
	m_pDevice = nullptr;
	m_pMouse = nullptr;
	m_pKeyboard = nullptr;
}

std::vector< CButton* >& CMenu::GetButtons()
{
	return ( m_vecButtons );
}

void CMenu::Initialise()
{
	// Get mouse from services
	void* vpMouse;
	CServices::GetService( SERVICETYPE_MOUSE, vpMouse );
	m_pMouse = static_cast< CMouseInput* >( vpMouse );
	
	// Get keyboard from services
	void* vpKeyboard;
	CServices::GetService( SERVICETYPE_KEYBOARD, vpKeyboard );
	m_pKeyboard = static_cast< CKeyboardInput* >( vpKeyboard );

	// Get d3d device from services
	void* vpDevice;
	CServices::GetService( SERVICETYPE_D3DDEVICE, vpDevice );
	m_pDevice = static_cast< ID3D10Device* >( vpDevice );	

	InitialiseBuffer();
}
	
void CMenu::InitialiseBuffer()
{
	// Adds the buttons to the vertices
	for( unsigned i = 0; i < m_vecButtons.size(); ++i )
	{
		m_vecVertices.push_back( m_vecButtons[ i ] );
	}

	// Adds the text to the vertices
	for( unsigned i = 0; i < m_vecStaticText.size(); ++i )
	{
		m_vecVertices.push_back( m_vecStaticText[ i ] );
	}

	// Adds the text boxes to the vertices
	for( unsigned i = 0; i < m_vecTextBox.size(); ++i )
	{
		m_vecVertices.push_back( m_vecTextBox[ i ] );
	}
	
	// Doesn't create the vertex buffer if not needed
	if( m_vecVertices.size() > 0 )
	{	
		// Used to give actual values to the buffer, not pointers
		std::vector< CPointSprite > vecInitial;
		for( unsigned i = 0; i < m_vecVertices.size(); ++i )
		{
			vecInitial.push_back( *m_vecVertices[ i ] );
		}

		// Fill description for buffer
		D3D10_BUFFER_DESC VBDesc;
		VBDesc.Usage			= D3D10_USAGE_DYNAMIC;
		VBDesc.ByteWidth		= sizeof( CPointSprite ) * vecInitial.size();
		VBDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
		VBDesc.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE;
		VBDesc.MiscFlags		= 0;
	
		D3D10_SUBRESOURCE_DATA VInitData;
		VInitData.pSysMem = vecInitial.data();

		// Create buffer
		HR( m_pDevice->CreateBuffer( &VBDesc, &VInitData, &m_pVertexBuffer ) );
	}
}

void CMenu::MapBuffer()
{
	// Incase no vertex buffer needed for menu
	if( m_pVertexBuffer != nullptr )
	{
		// Maps the updated vertices to the buffer
		CPointSprite* pSprite = nullptr;
		m_pVertexBuffer->Map( D3D10_MAP_WRITE_DISCARD, 0, ( void** ) &pSprite );

		for( unsigned i = 0; i < m_vecVertices.size(); ++ i )
		{
			pSprite[ i ] = *m_vecVertices[ i ];
		}

		m_pVertexBuffer->Unmap();
	}
}

void CMenu::Update( float _fDT )
{
	// Loop through text boxes
	for( unsigned i = 0; i < m_vecTextBox.size(); ++i )
	{
		// If the mouse is down check if it is being pressed
		// set it to be selected if it is
		if( m_pMouse->LeftPressed() )
		{
			bool bInBox = m_vecTextBox[ i ]->InHitBox( m_pMouse->GetPosition() );
			m_vecTextBox[ i ]->SetSelected( bInBox );
		}

		m_vecTextBox[ i ]->Update();
	}

	// Loop through the buttons 
	for( unsigned i = 0; i < m_vecButtons.size(); ++i )
	{
		// Check if it has been selected and execute its function
		if( m_vecButtons[ i ]->InHitBox( m_pMouse->GetPosition() ) && m_pMouse->LeftPressed() )
		{
			m_vecButtons[ i ]->ExecuteFunction();
		}
	}
	
	MapBuffer();
}

void CMenu::Draw()
{
	// Draw the point sprites of the UI elements
	UINT stride = sizeof( CPointSprite );
	UINT offset = 0; 
	m_pDevice->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	m_pDevice->Draw( m_vecVertices.size(), 0 );

	// Output the text of the elements
	for( unsigned i = 0; i < m_vecButtons.size(); ++i )
	{
		m_vecButtons[ i ]->OutputText();
	}
	
	for( unsigned i = 0; i < m_vecStaticText.size(); ++i )
	{
		m_vecStaticText[ i ]->OutputText();
	}

	for( unsigned i = 0; i < m_vecTextBox.size(); ++i )
	{
		m_vecTextBox[ i ]->OutputText();
	}
}

void CMenu::AddButton( CButton* _pButton )
{
	m_vecButtons.push_back( _pButton );
}

void CMenu::AddText( CStaticText* _pText )
{
	m_vecStaticText.push_back( _pText );
}

void CMenu::AddTextBox( CTextBox* _pTextBox )
{
	m_vecTextBox.push_back( _pTextBox );
}