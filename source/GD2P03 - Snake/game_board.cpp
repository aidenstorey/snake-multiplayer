//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: game_board.cpp
// Description	: Handles the state of a game and its management
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

// Local Includes
#include "services.h"

// This Include
#include "game_board.h"

// Implementation
CGameBoard::CGameBoard() :
	m_pDevice( nullptr ), m_pVertexBuffer( nullptr ),
	m_tBoardSize( 0, 0 ), m_vBoardAnchor( 0.0f, 0.0f ),
	m_vBoardSeperation( 0.0f, 0.0f ), m_vBoardScale( 0.0f, 0.0f ),
	m_iBoard( nullptr ), m_tFruitPosition( 0, 0 ),m_bFruitReset( false )
{}

CGameBoard::~CGameBoard()
{
	// Deletes board
	for( int i = 0; i < m_tBoardSize.y; ++i )
	{
		delete [] m_iBoard[ i ];
	}
	delete [] m_iBoard;

	ReleaseCOM( m_pVertexBuffer );
}

int CGameBoard::GetUnitState( TGridPoint _tPosition ) const
{
	// Sets default state to be empty
	int iReturn = g_BoardStateEmpty;
	
	// If on the board set the return to be unit state
	if( _tPosition < m_tBoardSize && _tPosition >= TGridPoint( 0, 0 ) )
	{
		iReturn = m_iBoard[ _tPosition.y ][ _tPosition.x ];
	}
	return ( iReturn );
}

void CGameBoard::SetUnitState( TGridPoint _tPosition, int _iState )
{
	// Sets the state of unit if on the board
	if( _tPosition < m_tBoardSize && _tPosition >= TGridPoint( 0, 0 ) )
	{
		m_iBoard[ _tPosition.y ][ _tPosition.x ] = _iState;
		// If the vertices have been initialised update the color too
		// ( They won't be initialised on a server client so hence the check )
		if( m_vecVertices.size() > 0 )
		{
			m_vecVertices[ _tPosition.y * m_tBoardSize.y + _tPosition.x ].SetColor( StateColor( _iState ) );
		}
	}
}

TGridPoint CGameBoard::GetSize() const
{
	return ( m_tBoardSize );
}

D3DXVECTOR2 CGameBoard::GetAnchor() const
{
	return ( m_vBoardAnchor );
}

void CGameBoard::SetAnchor( const D3DXVECTOR2 _vAnchor )
{
	m_vBoardAnchor = _vAnchor;
}

D3DXVECTOR2 CGameBoard::GetSeperation() const
{
	return ( m_vBoardSeperation );
}

void CGameBoard::SetSeperation( const D3DXVECTOR2 _vSeperation )
{
	m_vBoardSeperation = _vSeperation;
}

D3DXVECTOR2 CGameBoard::GetScale() const
{
	return ( m_vBoardScale );
}

void CGameBoard::SetScale( const D3DXVECTOR2 _vScale )
{
	m_vBoardScale = _vScale;
}

TGridPoint CGameBoard::GetFruitPosition() const
{
	return ( m_tFruitPosition );
}

void CGameBoard::SetFruitPosition( const TGridPoint _tPosition )
{
	// Sets the current fruit position to empty and then sets the
	// new fruit position to taken
	SetUnitState( m_tFruitPosition, g_BoardStateEmpty );
	m_tFruitPosition = _tPosition;	
	SetUnitState( m_tFruitPosition, g_BoardStateFruit );
}

std::string CGameBoard::GetStateAsString() const
{
	std::string sBoardState;

	// Loops over the board packaging it ready to be sent via network
	for( int i = 0; i < m_tBoardSize.y; ++i )
	{
		for( int j = 0; j < m_tBoardSize.x; ++j )
		{
			sBoardState += StateToValue( m_iBoard[ i ][ j ] );
		}
	}

	return ( sBoardState );
}

void CGameBoard::SetStateFromString( const std::string _State )
{
	// Parses the string ans sets the board state accordingly
	for( unsigned i = 0; i < _State.size(); ++i )
	{
		SetUnitState( TGridPoint( i / m_tBoardSize.y, i % m_tBoardSize.y ), ValueToState( _State[ i ] ) );
	}
}

bool CGameBoard::HasFruitReset()
{
	bool bReturn = m_bFruitReset;

	m_bFruitReset = false;

	return ( bReturn );
}

void CGameBoard::InitialiseAsClient( const TGridPoint _tBoardSize, const D3DXVECTOR2 _vAnchor, const D3DXVECTOR2 _vSeperation, const D3DXVECTOR2 _vScale  )
{	
	// Get device from services
	void* vpDevice;
	CServices::GetService( SERVICETYPE_D3DDEVICE, vpDevice );
	m_pDevice = static_cast< ID3D10Device* >( vpDevice );

	// Store local copy of parameters
	m_tBoardSize		= _tBoardSize;
	m_vBoardAnchor		= _vAnchor;
	m_vBoardSeperation	= _vSeperation;
	m_vBoardScale		= _vScale;

	// Create board Array
	m_iBoard = new int*[ m_tBoardSize.y ];

	for( int i = 0; i < m_tBoardSize.y; ++i )
	{
		m_iBoard[ i ] = new int[ m_tBoardSize.x ];
	}

	// Initialise board values
	for( int i = 0; i < m_tBoardSize.y; ++i )
	{
		for( int j = 0; j < m_tBoardSize.x; ++j )
		{
			m_iBoard[ i ][ j ] = g_BoardStateEmpty;

			D3DXVECTOR2 vPosition;
			vPosition.x = m_vBoardAnchor.x + ( j * _vSeperation.x );
			vPosition.y = m_vBoardAnchor.y - ( i * _vSeperation.y );
			m_vecVertices.push_back( CPointSprite( vPosition, m_vBoardScale, 0.0f, StateColor( g_BoardStateEmpty ) ) );
		}
	}

	// Ensures buffer not falsly created
	if( m_vecVertices.size() > 0 )
	{	
		// Fill description for buffer
		D3D10_BUFFER_DESC VBDesc;
		VBDesc.Usage			= D3D10_USAGE_DYNAMIC;
		VBDesc.ByteWidth		= sizeof( CPointSprite ) * m_vecVertices.size();
		VBDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
		VBDesc.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE;
		VBDesc.MiscFlags		= 0;
	
		D3D10_SUBRESOURCE_DATA VInitData;
		VInitData.pSysMem = m_vecVertices.data();

		// Create buffer
		HR( m_pDevice->CreateBuffer( &VBDesc, &VInitData, &m_pVertexBuffer ) );
	}
}

void CGameBoard::InitialiseAsServer( const TGridPoint _tBoardSize )
{
	// Store local copy of parameters
	m_tBoardSize		= _tBoardSize;
	
	// Create board Array
	m_iBoard = new int*[ m_tBoardSize.y ];

	for( int i = 0; i < m_tBoardSize.y; ++i )
	{
		m_iBoard[ i ] = new int[ m_tBoardSize.x ];
	}

	// Initialise board values
	for( int i = 0; i < m_tBoardSize.y; ++i )
	{
		for( int j = 0; j < m_tBoardSize.x; ++j )
		{
			m_iBoard[ i ][ j ] = g_BoardStateEmpty;
		}
	}

	// Set initial fruit position
	m_tFruitPosition.x = rand() % m_tBoardSize.x;
	m_tFruitPosition.y = rand() % m_tBoardSize.y;
	SetFruitPosition( m_tFruitPosition );
}

bool CGameBoard::IsOnBoard( TGridPoint _tPosition ) const
{
	return ( _tPosition < m_tBoardSize && _tPosition >= TGridPoint( 0, 0 ) );
}

bool CGameBoard::CanMove( TGridPoint _tPosition ) const
{
	bool bReturn = false;

	// Checks if the position is on the board and if it is occupied or not returning result
	if( IsOnBoard( _tPosition ) )
	{
		bReturn = m_iBoard[ _tPosition.y ][ _tPosition.x ] == g_BoardStateEmpty || m_iBoard[ _tPosition.y ][ _tPosition.x ] == g_BoardStateFruit;
	}
	
	return ( bReturn );
}

void CGameBoard::ResetFruit()
{
	TGridPoint tFruitPos = m_tFruitPosition;

	// Sets the fruit position to an empty unit on the board
	while( m_iBoard[ tFruitPos.y ][ tFruitPos.x ] != g_BoardStateEmpty )
	{
		tFruitPos.x = rand() % m_tBoardSize.x;
		tFruitPos.y = rand() % m_tBoardSize.y;
	}

	m_bFruitReset = true;

	SetFruitPosition( tFruitPos );
}

char CGameBoard::StateToValue( int _iState ) const
{
	// Offsets state to start from A
	return ( _iState + g_BoardStateValueOffset + g_BoardStateValueInit );
}

int CGameBoard::ValueToState( char _Value ) const
{
	// Derives state from value by taking off ofset and inital pass value
	return ( _Value - g_BoardStateValueOffset - g_BoardStateValueInit);
}

D3DXCOLOR CGameBoard::StateColor( const int _iState )
{
	D3DXCOLOR vReturn = g_BoardColorEmpty;

	// Returns the color based on which state type passed in
	if( _iState >= 0 )
	{
		vReturn = g_BoardColorSnakes[ _iState ];
	}
	else if ( _iState == g_BoardStateFruit )
	{
		vReturn = g_BoardColorFruit;
	}
	else if ( _iState == g_BoardStateDeadSnake )
	{
		vReturn = g_BoardColorSnakeDead;
	}	

	return ( vReturn );
}

void CGameBoard::Update()
{
	MapBuffer();
}

void CGameBoard::MapBuffer()
{
	// Incase no vertex buffer needed for menu
	if( m_pVertexBuffer != nullptr )
	{
		// Maps the updated vertices to the buffer
		CPointSprite* pSprite = nullptr;
		m_pVertexBuffer->Map( D3D10_MAP_WRITE_DISCARD, 0, ( void** ) &pSprite );

		for( unsigned i = 0; i < m_vecVertices.size(); ++ i )
		{
			pSprite[ i ] = m_vecVertices[ i ];
		}

		m_pVertexBuffer->Unmap();
	}
}

void CGameBoard::Draw()
{
	UINT stride = sizeof( CPointSprite );
	UINT offset = 0; 
	m_pDevice->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	m_pDevice->Draw( m_vecVertices.size(), 0 );
}