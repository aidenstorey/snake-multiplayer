//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: snake_game.cpp
// Description	: Handles both client and server running of the application
// Author		: Aiden Storey, Jacob Dodunski
// Mail			: aiden.storey@mediadesign.school.nz, Jacob.Dodunski@mediadesign.school.nz
//

// Library Includes
#include <string>

// Local Includes
#include "services.h"

// This Include
#include "snake_game.h"

// Implementation
CSnakeGame::CSnakeGame() :
	m_pDevice( nullptr ), m_pVertexBuffer( nullptr ),m_pKeyboard( nullptr ), 
	m_iNumPlayers( 0 ), m_fTimePassed( 0.0f ), m_pGameBoard( nullptr ), 
	m_pYourScore( nullptr ), m_pHighScore( nullptr )
{
	for( int i = 0; i < g_SnakeGameMaxNumPlayers; ++i )
	{
		m_pPlayers[ i ] = nullptr;
	}
}

CSnakeGame::~CSnakeGame()
{
	ReleaseCOM( m_pVertexBuffer );

	if( m_pGameBoard != nullptr )
	{
		delete m_pGameBoard;
		m_pGameBoard = nullptr;
	}

	for( int i = 0; i < g_SnakeGameMaxNumPlayers; ++i )
	{
		if( m_pPlayers[ i ] != nullptr )
		{
			delete m_pPlayers[ i ];
			m_pPlayers[ i ] = nullptr;
		}
	}

	if( m_pYourScore != nullptr )
	{
		delete m_pYourScore;
		m_pYourScore = nullptr;
	}

	if( m_pHighScore != nullptr )
	{
		delete m_pHighScore;
		m_pHighScore = nullptr;
	}
	m_vecVertices.clear();
}

std::string CSnakeGame::GetName() const
{
	return ( m_Name );
}

void CSnakeGame::SetName( const std::string _Name )
{
	m_Name = _Name;
}

std::string CSnakeGame::GetScores() const
{
	std::string sReturn = "";

	// Write all users to a string to be sent
	for( unsigned i = 0; i < m_Snakes.size(); ++i )
	{
		char buffer[33];
		_itoa_s(int(m_Snakes[ i ].GetLength()), buffer, 10);
		std::string sScore = buffer;

		sReturn += m_Snakes[ i ].GetName().c_str();
		sReturn += ":";
		sReturn += sScore;
		sReturn += ",";
	}

	return ( sReturn );
}

void CSnakeGame::ChangeSnakeDirection( unsigned _SnakeNum, ESnakeDirection _eDirection )
{
	// Change snake if valid
	if( _SnakeNum < m_Snakes.size() )
	{
		m_Snakes[ _SnakeNum ].ChangeDirection( _eDirection );
	}
}

void CSnakeGame::InitialiseAsClient( std::string _Name, D3DXVECTOR2 _vTextOffset, ID3DX10Font* _pFont, std::string _BoardState )
{	
	// Local copies
	m_Name			= _Name;
	m_vTextOffset	= _vTextOffset;
	m_pFont			= _pFont;

	// Set state to client
	m_bServer = false;

	// Get keyboard from services
	void* vpKeyboard;
	CServices::GetService( SERVICETYPE_KEYBOARD, vpKeyboard );
	m_pKeyboard = static_cast< CKeyboardInput* >( vpKeyboard );

	// Get device from services
	void* vpDevice;
	CServices::GetService( SERVICETYPE_D3DDEVICE, vpDevice );
	m_pDevice = static_cast< ID3D10Device* >( vpDevice );

	// Get network from services
	void* vpNetwork;
	CServices::GetService( SERVICETYPE_NETWORKDEVICE, vpNetwork );
	m_pNetwork = static_cast< Network* >( vpNetwork );

	// Initialise the board
	m_pGameBoard = new CGameBoard();
	m_pGameBoard->InitialiseAsClient( g_SnakeGameBoardSize, g_SnakeGameBoardAnchor, g_SnakeGameBoardSeperation, g_SnakeGameScale );
	m_pGameBoard->SetStateFromString( _BoardState );

	// Initialise the text for the board
	InitialiseText();

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

void CSnakeGame::InitialiseAsServer()
{
	// Set to client Instance
	m_bServer = true;

	// Get network from services
	void* vpNetwork;
	CServices::GetService( SERVICETYPE_NETWORKDEVICE, vpNetwork );
	m_pNetwork = static_cast< Network* >( vpNetwork );

	// Initialise the baord
	m_pGameBoard = new CGameBoard();
	m_pGameBoard->InitialiseAsServer( g_SnakeGameBoardSize );

	// Get the users connected to the client
	std::vector< TUser > users = m_pNetwork->GetClients();
	m_iNumPlayers = users.size();
	
	// Initialise the fruits position
	m_pGameBoard->ResetFruit();

	// Set the range that the player can spawn in to ensure they don't run
	// into walls as soon as the game starts
	int iPosRangeMax = g_SnakeGameBoardSize.x - 5;
	int iPosRangeMin = g_SnakeGameBoardSize.x - iPosRangeMax;

	// Loop over the number of players creating a new snake at a random
	// empty position for them, initialising it
	for( int i = 0; i < m_iNumPlayers; ++i )
	{
		TGridPoint startPos;

		do
		{
			startPos.Randomize( iPosRangeMin, iPosRangeMax );
		}
		while( m_pGameBoard->GetUnitState( startPos ) != g_BoardStateEmpty );

		m_Snakes.push_back( CPlayerSnake() );
		m_Snakes.back().Initialise( users[ i ].t_Username, m_pGameBoard, startPos, g_SnakeGameInitialLength, 
			static_cast< ESnakeDirection >( GenerateRandom( 0, SDIR_MAX ) ), i );		
	}

	// Send the game state to the clients and the scores
	m_pNetwork->Send( STATE, m_pGameBoard->GetStateAsString() );
	m_pNetwork->Send( SCORES, GetScores() );
	Sleep( 1000 ); // Sleep to let players see where they are
}

void CSnakeGame::InitialiseText()
{
	// Creates the score text for display
	m_pYourScore = new CStaticText( D3DXVECTOR2( -130.0f, 125.0f ), D3DXVECTOR2( 110.0f, 15.0f ), "Score: ", m_vTextOffset, m_pFont, D3DXCOLOR( 0.4f, 0.4f, 0.4f, 1.0f ) ); 
	m_vecVertices.push_back( m_pYourScore );

	// Creates the high score text for display
	m_pHighScore = new CStaticText( D3DXVECTOR2( 15.0f, 125.0f ), D3DXVECTOR2( 140.0f, 15.0f ), "Highscore: ", m_vTextOffset, m_pFont, D3DXCOLOR( 0.4f, 0.4f, 0.4f, 1.0f ) ); 
	m_vecVertices.push_back( m_pHighScore ); 

	// Creates the names + score holders for display
	for( int i = 0; i < g_SnakeGameMaxNumPlayers; ++i )
	{
		D3DXVECTOR2 pos = g_SnakeGameScoresAnchor + D3DXVECTOR2( 0.0f, -18.0f * i );
		m_pPlayers[ i ] = new CStaticText( pos, D3DXVECTOR2( 95.0f, 15.0f ), "", m_vTextOffset, m_pFont, g_BoardColorSnakes[ i ] );
		m_vecVertices.push_back( m_pPlayers[ i ] );
	}
}

void CSnakeGame::MapBuffer()
{
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

void CSnakeGame::Update( float _fDT )
{
	// If the client is a server
	if( m_bServer )
	{
		// Update the time passed
		m_fTimePassed += _fDT;

		// If the time passed has reached a given value
		if( m_fTimePassed > 1.0f / 10.0f )
		{
			// Reset time passed
			m_fTimePassed = 0.0f;

			// move all of the snakes
			for( unsigned i = 0; i < m_Snakes.size(); ++i )
			{
				m_Snakes[ i ].Move();
			}

			// Send the new board state to all of the clients
			m_pNetwork->Send( STATE, m_pGameBoard->GetStateAsString() );

			if( m_pGameBoard->HasFruitReset() )
			{
				m_pNetwork->Send( SCORES, GetScores() );
			}
		}
	}
	else
	{
		CheckKeyboard();

		// Re-maps buffer and updates the board
		MapBuffer();
		m_pGameBoard->Update();
	}
}


void CSnakeGame::SetBoard( std::string _boardState )
{
	m_pGameBoard->SetStateFromString( m_pNetwork->GetTextMessage() );
}

void CSnakeGame::UpdateText( std::string _scores )
{
	// Parses the scores from a string
	std::string sTemp = "";

	int iHighest = 0;

	std::string sName = "";
	std::string sScore = "";

	unsigned iPlayerIndex = 0;

	unsigned iIndex = 0; 
	while( iIndex < _scores.length() )
	{
		// If the character is a : store out the name
		if( _scores.c_str()[ iIndex ] == ':' )
		{
			sName = sTemp;
			sTemp = "";
			iIndex++;
		}
		// If the character is a , the parsing for that player is done
		else if( _scores.c_str()[ iIndex ] == ',' )
		{
			sScore = sTemp;
			sTemp = "";

			// If the name of the player is the same as the one stored on client
			// Updated the score textbox as it is your score
			if( std::strcmp(sName.c_str(), m_Name.c_str()) == 0 )
			{
				std::string sYourScore = "Score: ";
				sYourScore += sScore;
				m_pYourScore->SetButtonText( sYourScore );
			}

			// If the score is higher than the current highest
			// update the highest score 
			if( atoi( sScore.c_str() ) > iHighest )
			{
				std::string sHighScore = "Highscore: ";
				sHighScore += sScore;
				m_pHighScore->SetButtonText( sHighScore );
			}

			// Update the players score on the overall scores
			std::string sPlayerScore = sName;
			sPlayerScore += " ";
			sPlayerScore += sScore;
			m_pPlayers[ iPlayerIndex ]->SetButtonText( sPlayerScore );

			iPlayerIndex++;
			iIndex++;
		}
		else
		{
			sTemp += _scores[ iIndex ];
			iIndex++;
		}
	}
}

void CSnakeGame::Draw()
{	
	// Draw the point sprites
	UINT stride = sizeof( CPointSprite );
	UINT offset = 0;
	m_pDevice->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	m_pDevice->Draw( m_vecVertices.size(), 0 );

	// Draw the board
	m_pGameBoard->Draw();

	// Ouput the two texts
	m_pYourScore->OutputText();
	m_pHighScore->OutputText();

	// If tab is being held output the highscores of everyone
	if( m_pKeyboard->KeyDown( VK_TAB ) )
	{
		for( int i = 0; i < g_SnakeGameMaxNumPlayers; ++i )
		{
			m_pPlayers[ i ]->OutputText();
		}
	}
}

bool CSnakeGame::IsOver()
{
	bool bReturn = true;

	// If there are no snakes left the game is over
	for( unsigned i = 0; i < m_Snakes.size(); ++i )
	{
		if( m_Snakes[ i ].GetAlive() )
		{
			bReturn = false;
			break;
		}
	}

	return ( bReturn );
}

void CSnakeGame::ResetGame()
{
	// Release buffer
	ReleaseCOM( m_pVertexBuffer );

	// Set values
	m_iNumPlayers = 0;
	m_fTimePassed = 0.0f;
	
	// Clean up drawn objects
	for( unsigned i = 0; i < m_vecVertices.size(); ++i )
	{
		delete m_vecVertices[ i ];
	}
	m_vecVertices.clear();

	// Delete board
	if( m_pGameBoard != nullptr )
	{
		delete m_pGameBoard;
		m_pGameBoard = nullptr;
	}

	// Deletion handled by vertex clean up
	for( int i = 0; i < g_SnakeGameMaxNumPlayers; ++i )
	{
		m_pPlayers[ i ] = nullptr;
	}

	m_pYourScore = nullptr;
	m_pHighScore = nullptr;

	// Clean up std objects
	m_Name.clear();
	m_Snakes.clear();
}

void CSnakeGame::CheckKeyboard()
{
	if( m_pKeyboard->KeyPressed( 0x25 ) ) // Up
	{
		m_pNetwork->Send( UP, "" );
	}
	else if( m_pKeyboard->KeyPressed( 0x27 ) ) // Down
	{
		m_pNetwork->Send( DOWN, "" );
	}
	else if( m_pKeyboard->KeyPressed( 0x26 ) ) // Left
	{
		m_pNetwork->Send( LEFT, "" );
	}
	else if( m_pKeyboard->KeyPressed( 0x28 ) ) // Right
	{
		m_pNetwork->Send( RIGHT, "" );
	}
	
	if( m_pKeyboard->KeyPressed( VK_TAB ) )
	{
		m_pNetwork->Send( SCORES, "" );
	}
}