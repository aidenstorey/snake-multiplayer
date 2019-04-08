//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: player_snake.cpp
// Description	: Handles the creation and state of a player within the game world
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

// This Include
#include "player_snake.h"

// Implementation
CPlayerSnake::CPlayerSnake() :
	m_pGameBoard( false ), m_bAlive( false ), 
	m_eDirection( SDIR_NONE ), m_bCanChange( false ),
	m_iPlayerNum( 0 )
{}

CPlayerSnake::~CPlayerSnake()
{}

int CPlayerSnake::GetPlayerNum() const
{
	return ( m_iPlayerNum );
}

void CPlayerSnake::SetPlayerNum( const int _iPlayerNum )
{
	m_iPlayerNum = _iPlayerNum;
}

bool CPlayerSnake::GetAlive() const
{
	return ( m_bAlive );
}

void CPlayerSnake::SetAlive( const bool _bAlive )
{
	m_bAlive = _bAlive;
}

ESnakeDirection CPlayerSnake::GetDirection() const
{
	return ( m_eDirection );
}

void CPlayerSnake::SetDirection( const ESnakeDirection _eDiection )
{
	m_eDirection = _eDiection;
}

unsigned CPlayerSnake::GetLength() const
{
	return ( m_vecPosition.size() );
}

std::string CPlayerSnake::GetName() const
{
	return ( m_Name );
}

void CPlayerSnake::SetName( const std::string _Name )
{
	m_Name = _Name;
}

void CPlayerSnake::Initialise( std::string _Name, CGameBoard* _pGameBoard, const TGridPoint _StartPoint, const int _iStartLength, const ESnakeDirection _eDirection, const int _iPlayerNum )
{
	// Store local copy of parameters
	m_Name		= _Name;
	m_pGameBoard = _pGameBoard;
	m_eDirection = _eDirection;
	m_iPlayerNum = _iPlayerNum;

	for( int i = 0; i < _iStartLength; ++i )
	{
		m_vecPosition.push_back( _StartPoint );
		m_pGameBoard->SetUnitState( _StartPoint, m_iPlayerNum );
	}

	// Initialise variables
	m_bAlive		= true;
	m_bCanChange	= true;
}

void CPlayerSnake::ChangeDirection( const ESnakeDirection _eDirection )
{
	// Ensures that the snake doesn't change direction again before being processed
	if( m_bCanChange )
	{
		// Ensures that the snake doesn't go back on itself
		switch( _eDirection )
		{
		case SDIR_UP: { if( m_eDirection != SDIR_DOWN ){ m_eDirection = _eDirection; } }
			break;
		case SDIR_DOWN: { if( m_eDirection != SDIR_UP ){ m_eDirection = _eDirection; } }
			break;
		case SDIR_LEFT: { if( m_eDirection != SDIR_RIGHT ){ m_eDirection = _eDirection; } }
			break;
		case SDIR_RIGHT: { if( m_eDirection != SDIR_LEFT ){ m_eDirection = _eDirection; } }
			break;
		default: break;
		}

		m_bCanChange = false;
	}
}

void CPlayerSnake::AddUnit()
{
	m_vecPosition.push_back( m_vecPosition.back() );
}

void CPlayerSnake::Move()
{
	// Only moves if the snake is actually alive
	if( m_bAlive )
	{
		// Lets the direction be changed again
		m_bCanChange = true;
		TGridPoint tPosition = m_vecPosition[ 0 ];

		// Updates the position based on movement
		switch( m_eDirection )
		{
		case SDIR_UP: { --tPosition.y; }
			break;
		case SDIR_DOWN: { ++tPosition.y; }
			break;
		case SDIR_LEFT: { --tPosition.x; }
			break;
		case SDIR_RIGHT: { ++tPosition.x; }
			break;
		default: break; 
		}

		// Initialises the movement check
		bool bCanMove = true;
	
		// If it cannot move kill it and set all the board states to be dead
		if( !m_pGameBoard->CanMove( tPosition ) )
		{
			m_bAlive = false;
			bCanMove = false;

			for( unsigned i = 0; i < m_vecPosition.size(); ++i )
			{
				m_pGameBoard->SetUnitState( m_vecPosition[ i ], g_BoardStateDeadSnake );
			}
		}

		// if it can move
		if( bCanMove )
		{
			// Check if the state is a fruit and add a unit to the snake if it is
			if( m_pGameBoard->GetUnitState( tPosition ) == g_BoardStateFruit )
			{
				AddUnit();
				m_pGameBoard->ResetFruit();
			}
			else 
			{
				// Set the state of the last units current position to be empty
				m_pGameBoard->SetUnitState( m_vecPosition.back(), g_BoardStateEmpty );
			}

			// Move from the back of the snake setting its position to the unit
			// in front of its position
			for( unsigned i = m_vecPosition.size() - 1; i > 0; --i )
			{
				m_vecPosition[ i ] = m_vecPosition[ i - 1 ];
			}

			// Set the fronts position to the new position of the snake and update
			// the state of the baord at that position
			m_vecPosition.front() = tPosition;

			m_pGameBoard->SetUnitState( m_vecPosition.front(), m_iPlayerNum );
		}
	}
}
