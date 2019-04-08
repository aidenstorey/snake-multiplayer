//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: game_manager.cpp
// Description	: Handles the state of the program ( in menu, in game ) and how each element interacts with one another
// Author		: Aiden Storey, Jacob Dodunski
// Mail			: aiden.storey@mediadesign.school.nz, Jacob.Dodunski@mediadesign.school.nz
//

// Local Includes
#include "services.h"

// This Include 
#include "game_manager.h"

// Implementation
CGameManager::CGameManager() :
	m_bInGame( false ), m_pCurrentMenu( nullptr ),
	m_pTitleFont( nullptr ), m_pDefaultFont( nullptr ),
	m_fClientWidth( 0.0f ), m_fClientHeight( 0.0f ),
	m_pNetwork( nullptr ), m_pTextBox( nullptr ),
	m_eMenuType( MENU_NONE ), m_pFindButton( nullptr ),
	m_pHostButton( nullptr ), m_pReadyButton( nullptr )
{}

CGameManager::~CGameManager()
{
	m_pCurrentMenu = nullptr;
	ReleaseCOM( m_pTitleFont );
	ReleaseCOM( m_pDefaultFont );
}

void CGameManager::Initialise( int _iClientWidth, int _iClientHeight )
{
	// Store local copies of parameters
	m_fClientWidth	= static_cast< float >( _iClientWidth );
	m_fClientHeight = static_cast< float >( _iClientHeight ); 
	
	// Set value to move text origin to middle of screen
	m_vTextOffset = D3DXVECTOR2( m_fClientWidth / 2.0f , m_fClientHeight / 2.0f );

	// Initialise menu related objects
	InitialiseFonts();
	InitialiseMenues();

	// Initialise game state
	m_bInGame = false;

	// Get network from services
	void* vpNetwork;
	CServices::GetService( SERVICETYPE_NETWORKDEVICE, vpNetwork );
	m_pNetwork = static_cast< Network* >( vpNetwork );
}

void CGameManager::InitialiseFonts()
{
	// Get d3d device from services
	void* vpDevice;
	CServices::GetService( SERVICETYPE_D3DDEVICE, vpDevice );
	ID3D10Device* pDevice = static_cast< ID3D10Device* >( vpDevice );

	D3DX10_FONT_DESC fontDesc;
	fontDesc.Height				= 42;
    fontDesc.Width				= 0;
    fontDesc.Weight				= 0;
    fontDesc.MipLevels			= 1;
    fontDesc.Italic				= false;
    fontDesc.CharSet			= DEFAULT_CHARSET;
    fontDesc.OutputPrecision	= OUT_DEFAULT_PRECIS;
    fontDesc.Quality			= DEFAULT_QUALITY;
    fontDesc.PitchAndFamily		= DEFAULT_PITCH | FF_DONTCARE;
    wcscpy_s(fontDesc.FaceName, L"Consolas");
	
	// Create title font
	D3DX10CreateFontIndirect( pDevice, &fontDesc, &m_pTitleFont );
		
	// Change font height and create other font
	fontDesc.Height				= 14;
	D3DX10CreateFontIndirect( pDevice, &fontDesc, &m_pDefaultFont );
}

void CGameManager::InitialiseMenues()
{	
	// TODO:	If enough time change initialising menues to be reading from an external file

	// Intialise textbox for menu
	m_pTextBox = new CTextBox( D3DXVECTOR2( 0.0f, 40.0f ), D3DXVECTOR2( 130.0f, 20.0f ), m_vTextOffset, m_pDefaultFont );

	// Intialise buttons for menu
	m_pFindButton = new CButton( D3DXVECTOR2( 0.0f, 0.0f ), D3DXVECTOR2( 150.0f, 20.0f ), "Find Server", m_vTextOffset, m_pDefaultFont, this, &CGameManager::GoToFindServer, true );
	m_pHostButton = new CButton( D3DXVECTOR2( 0.0f, -25.0f ), D3DXVECTOR2( 150.0f, 20.0f ), "Host Server", m_vTextOffset, m_pDefaultFont, this, &CGameManager::GoToSever, true );
	m_pReadyButton = new CButton( D3DXVECTOR2( 0.0f, -100.0f ), D3DXVECTOR2( 150.0f, 20.0f ), "Ready", m_vTextOffset, m_pDefaultFont, this, &CGameManager::Ready, true );

	// Initialise Main
	m_MenuMain.AddButton( m_pFindButton );
	m_MenuMain.AddButton( m_pHostButton );
	m_MenuMain.AddButton( new CButton( D3DXVECTOR2( 0.0f, -50.0f ), D3DXVECTOR2( 150.0f, 20.0f ), "Instructions", m_vTextOffset, m_pDefaultFont, this, &CGameManager::GoToInstructions, true ) );
	m_MenuMain.AddButton( new CButton( D3DXVECTOR2( 0.0f, -75.0f ), D3DXVECTOR2( 150.0f, 20.0f ), "Quit", m_vTextOffset, m_pDefaultFont, this, &CGameManager::QuitApplication, true ) );
	m_MenuMain.AddText( new CStaticText( D3DXVECTOR2( 0.0f, 100.0f ), D3DXVECTOR2( 150.0f, 50.0f ), "Snake", m_vTextOffset, m_pTitleFont, BLACK ) );	
	m_MenuMain.AddTextBox( m_pTextBox );
	m_MenuMain.Initialise();
	
	// Initialise Instructions
	m_MenuInstructions.AddText( new CStaticText( D3DXVECTOR2( 0.0f, 100.0f ), D3DXVECTOR2( 300.0f, 50.0f ), "Instructions", m_vTextOffset, m_pTitleFont, BLACK ) );
	m_MenuInstructions.AddText( new CStaticText( D3DXVECTOR2( 0.0f, -5.0f ), D3DXVECTOR2( 300.0f, 110.0f ), 
		"How to play:\nMove around the game area\n with the arrow keys\ncollecting the fruit(red squares)\navoiding running into your own\nbody or others bodies\nand don't run into walls.\nHit 'Tab' in game to view scores.",
		m_vTextOffset, m_pDefaultFont, BLACK ) );
	m_MenuInstructions.AddButton( new CButton( D3DXVECTOR2( 0.0f, -125.0f ), D3DXVECTOR2( 150.0f, 20.0f ), "Return to Menu", m_vTextOffset, m_pDefaultFont, this, &CGameManager::GoToMain, true ) );
	m_MenuInstructions.Initialise();

	// Initialise Server menu
	m_MenuServer.AddText( new CStaticText( D3DXVECTOR2( 0.0f, 100.0f ), D3DXVECTOR2( 300.0f, 50.0f ), "Server Lobby", m_vTextOffset, m_pTitleFont, BLACK ) );
	InitialiseSeverText();
	m_MenuServer.Initialise();

	// Initialise Lobby
	m_MenuLobby.AddText( new CStaticText( D3DXVECTOR2( 0.0f, 100.0f ), D3DXVECTOR2( 300.0f, 50.0f ), "Game Lobby", m_vTextOffset, m_pTitleFont, BLACK ) );
	InitialiseLobbyText();
	
	m_MenuLobby.AddButton( m_pReadyButton );
	m_MenuLobby.AddButton( new CButton( D3DXVECTOR2( 0.0f, -125.0f ), D3DXVECTOR2( 150.0f, 20.0f ), "Return to Menu", m_vTextOffset, m_pDefaultFont, this, &CGameManager::GoToMain, true ) );
	m_MenuLobby.Initialise();	

	// Initialise Find Server
	m_MenuFindServer.AddText( new CStaticText( D3DXVECTOR2( 0.0f, 100.0f ), D3DXVECTOR2( 300.0f, 50.0f ), "Find Server", m_vTextOffset, m_pTitleFont, BLACK ) );
	m_MenuFindServer.AddButton( new CButton( D3DXVECTOR2( 0.0f, 50.0f ), D3DXVECTOR2( 150.0f, 20.0f ), "No server found", m_vTextOffset, m_pDefaultFont, this, &CGameManager::ConnectToServer, false ) );
	m_MenuFindServer.AddButton(	new CButton( D3DXVECTOR2( 0.0f, 25.0f ), D3DXVECTOR2( 150.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, this, &CGameManager::ConnectToServer, false ) );
	m_MenuFindServer.AddButton( new CButton( D3DXVECTOR2( 0.0f, 0.0f ), D3DXVECTOR2( 150.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, this, &CGameManager::ConnectToServer, false ) );
	m_MenuFindServer.AddButton(	new CButton( D3DXVECTOR2( 0.0f, -25.0f ), D3DXVECTOR2( 150.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, this, &CGameManager::ConnectToServer, false ) );
	m_MenuFindServer.AddButton(	new CButton( D3DXVECTOR2( 0.0f, -50.0f ), D3DXVECTOR2( 150.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, this, &CGameManager::ConnectToServer, false ) );
	m_MenuFindServer.AddButton( new CButton( D3DXVECTOR2( 0.0f, -125.0f ), D3DXVECTOR2( 150.0f, 20.0f ), "Return to Menu", m_vTextOffset, m_pDefaultFont, this, &CGameManager::GoToMain, true ) );
	m_MenuFindServer.Initialise();

	// Initialises Game over menu
	m_MenuOver.AddText( new CStaticText( D3DXVECTOR2( 0.0f, 0.0f ), D3DXVECTOR2( 300.0f, 50.0f ), "Game Over!", m_vTextOffset, m_pTitleFont, BLACK ) );
	m_MenuFindServer.AddButton( new CButton( D3DXVECTOR2( 0.0f, -125.0f ), D3DXVECTOR2( 150.0f, 20.0f ), "Return to Menu", m_vTextOffset, m_pDefaultFont, this, &CGameManager::GoToMain, true ) );
	m_MenuOver.Initialise();

	// Set initial menu
	m_pCurrentMenu = &m_MenuMain;
	
	// Set current menu state
	m_eMenuType = MENU_MAIN;
}

void CGameManager::InitialiseLobbyText()
{
	// Creates back color for all current text boxes
	D3DXCOLOR BackColor = D3DXCOLOR( 0.2f, 0.2f, 0.2f, 1.0f );

	// Creates lobby related text boxes
	m_pLobbyConnections[ 0 ]	= new CStaticText( D3DXVECTOR2( -100.0f, 60.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 0 ] );
	m_pLobbyConnections[ 1 ]	= new CStaticText( D3DXVECTOR2( 0.0f, 60.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 1 ] );
	m_pLobbyConnections[ 2 ]	= new CStaticText( D3DXVECTOR2( 100.0f, 60.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 2 ] );
	m_pLobbyConnections[ 3 ]	= new CStaticText( D3DXVECTOR2( -100.0f, 35.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 3 ] );
	m_pLobbyConnections[ 4 ]	= new CStaticText( D3DXVECTOR2( 0.0f, 35.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 4 ] );
	m_pLobbyConnections[ 5 ]	= new CStaticText( D3DXVECTOR2( 100.0f, 35.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 5 ] );
	m_pLobbyConnections[ 6 ]	= new CStaticText( D3DXVECTOR2( -100.0f, 10.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 6 ] );
	m_pLobbyConnections[ 7 ]	= new CStaticText( D3DXVECTOR2( 0.0f, 10.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 7 ] );
	m_pLobbyConnections[ 8 ]	= new CStaticText( D3DXVECTOR2( 100.0f, 10.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 8 ] );
	m_pLobbyConnections[ 9 ]	= new CStaticText( D3DXVECTOR2( -100.0f, -15.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 9 ] );
	m_pLobbyConnections[ 10 ]	= new CStaticText( D3DXVECTOR2( 0.0f, -15.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 10 ] );
	m_pLobbyConnections[ 11 ]	= new CStaticText( D3DXVECTOR2( 100.0f, -15.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 11 ] );
	m_pLobbyConnections[ 12 ]	= new CStaticText( D3DXVECTOR2( -100.0f, -40.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 12 ] );
	m_pLobbyConnections[ 13 ]	= new CStaticText( D3DXVECTOR2( 0.0f, -40.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 13 ] );
	m_pLobbyConnections[ 14 ]	= new CStaticText( D3DXVECTOR2( 100.0f, -40.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 14 ] );
	m_pLobbyConnections[ 15 ]	= new CStaticText( D3DXVECTOR2( 0.0f, -65.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 15 ] );

	// Give the menu the text boxes
	for( unsigned i = 0; i < g_SnakeGameMaxNumPlayers; ++i )
	{
		m_MenuLobby.AddText( m_pLobbyConnections[ i ] );
	}
}

void CGameManager::InitialiseSeverText()
{
	// Creates server related text boxes
	m_pServerConnections[ 0 ]	= new CStaticText( D3DXVECTOR2( -100.0f, 40.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 0 ] );
	m_pServerConnections[ 1 ]	= new CStaticText( D3DXVECTOR2( 0.0f, 40.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 1 ] );
	m_pServerConnections[ 2 ]	= new CStaticText( D3DXVECTOR2( 100.0f, 40.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 2 ] );
	m_pServerConnections[ 3 ]	= new CStaticText( D3DXVECTOR2( -100.0f, 15.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 3 ] );
	m_pServerConnections[ 4 ]	= new CStaticText( D3DXVECTOR2( 0.0f, 15.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 4 ] );
	m_pServerConnections[ 5 ]	= new CStaticText( D3DXVECTOR2( 100.0f, 15.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 5 ] );
	m_pServerConnections[ 6 ]	= new CStaticText( D3DXVECTOR2( -100.0f, -10.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 6 ] );
	m_pServerConnections[ 7 ]	= new CStaticText( D3DXVECTOR2( 0.0f, -10.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 7 ] );
	m_pServerConnections[ 8 ]	= new CStaticText( D3DXVECTOR2( 100.0f, -10.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 8 ] );
	m_pServerConnections[ 9 ]	= new CStaticText( D3DXVECTOR2( -100.0f, -35.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 9 ] );
	m_pServerConnections[ 10 ]	= new CStaticText( D3DXVECTOR2( 0.0f, -35.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 10 ] );
	m_pServerConnections[ 11 ]	= new CStaticText( D3DXVECTOR2( 100.0f, -35.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 11 ] );
	m_pServerConnections[ 12 ]	= new CStaticText( D3DXVECTOR2( -100.0f, -60.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 12 ] );
	m_pServerConnections[ 13 ]	= new CStaticText( D3DXVECTOR2( 0.0f, -60.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 13 ] );
	m_pServerConnections[ 14 ]	= new CStaticText( D3DXVECTOR2( 100.0f, -60.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 14 ] );
	m_pServerConnections[ 15 ]	= new CStaticText( D3DXVECTOR2( 0.0f, -85.0f ), D3DXVECTOR2( 95.0f, 20.0f ), "", m_vTextOffset, m_pDefaultFont, g_BoardColorSnakes[ 15 ] );

	// Give the meno the text boxes
	for( unsigned i = 0; i < g_SnakeGameMaxNumPlayers; ++i )
	{
		m_MenuServer.AddText( m_pServerConnections[ i ] );
	}

}

void CGameManager::Update( float _fDT )
{
	// Update game based on current state
	if( m_bInGame )
	{
		m_Game.Update( _fDT );
		if( m_pNetwork->IsServer() && m_Game.IsOver() )
		{
			m_pNetwork->Send( QUIT, "USERS" );
			m_Game.ResetGame();
			m_bInGame = false;
			GoToMain();
		}
	}
	else
	{
		// Set enabled of buttons based on if there is text in the text box
		m_pFindButton->SetEnabled( m_pTextBox->GetText() != "\0" );
		m_pHostButton->SetEnabled( m_pTextBox->GetText() != "\0" );

		// Update the current menu 
		m_pCurrentMenu->Update( _fDT );

		// Processes depending on state of menu
		switch( m_eMenuType )
		{
		case MENU_SERV:
			{
				OutputConnectedClients();
				if( m_pNetwork->IsGameReady() )
				{
					m_Game.InitialiseAsServer();
					m_bInGame = true;
				}
			}
			break;
		}
	}

	// Checks for and handles packets
	HandlePacket();
}

void CGameManager::Draw()
{
	// Draw menu or game depending on current state
	if( m_bInGame && !m_pNetwork->IsServer() )
	{
		m_Game.Draw();
	}
	else
	{
		m_pCurrentMenu->Draw();
	}
}

void CGameManager::GoToMain()
{
	// Set current state to be main menu
	m_pCurrentMenu = &m_MenuMain;	
	
	// Send quit message to ensure that if connection has occured it can be handled
	m_pNetwork->Send( QUIT, "" );
	
	// Reset the network to unbind the port
	m_pNetwork->Reset();

	// Ready UI
	m_eMenuType = MENU_MAIN;
	m_pReadyButton->SetEnabled( true );
}

void CGameManager::GoToInstructions()
{
	// Set current stat to be instructions menu
	m_pCurrentMenu = &m_MenuInstructions;
	m_eMenuType = MENU_INST;
}

void CGameManager::GoToFindServer()
{
	// Set the current state to be find server
	m_pCurrentMenu = &m_MenuFindServer;
	
	// Set up the network as a client
	m_pNetwork->Setup( m_pTextBox->GetText(), false );

	// Broad cast to find servers and get a list of active servers found
	std::vector< std::string > vecServers = m_pNetwork->Broadcast();

	// Get the buttons from find sever menu to update text
	std::vector< CButton* > vecButtons = m_MenuFindServer.GetButtons();

	// Set first button text to no server found incase no servers returned
	vecButtons[ 0 ]->SetButtonText( "No Server Found" );
	vecButtons[ 0 ]->SetEnabled( false );

	// Disable all other buttons
	for( unsigned i = 1; i < vecButtons.size() - 1; ++i )
	{
		vecButtons[ i ]->SetButtonText( "" );
		vecButtons[ i ]->SetEnabled( false );
	}

	// Enable buttons and set to server name if a server is found
	if( vecServers.size() > 0 )
	{
		for( unsigned i = 0; i < vecServers.size(); ++i )
		{
			vecButtons[ i ]->SetButtonText( vecServers[ i ] );
			vecButtons[ i ]->SetEnabled( true );
		}
	}
	
	m_eMenuType = MENU_FIND;
}

void CGameManager::GoToLobby()
{
	if( m_eMenuType != MENU_LOBB )
	{
		// Sets the state to lobby
		m_pCurrentMenu = &m_MenuLobby;
		m_eMenuType = MENU_LOBB;

		// Requests the currently connected users
		m_pNetwork->Send( USERS, "" );
	}
}

void CGameManager::GoToSever()
{
	// Sets up the instance to be a server
	m_pNetwork->Setup( m_pTextBox->GetText(), true );

	// Sets current stat to be server menu
	m_pCurrentMenu = &m_MenuServer;
	m_eMenuType = MENU_SERV;
}

void CGameManager::QuitApplication()
{
	PostQuitMessage( 0 );
}

void CGameManager::ConnectToServer()
{
	// Gets the buttons from the find server
	std::vector< CButton* > vecButtons = m_MenuFindServer.GetButtons();
	
	// Loop through them finding which one is pressed
	for( unsigned i = 0; i < vecButtons.size(); ++ i )
	{
		if( vecButtons[ i ]->IsBeingPressed() )
		{
			// If it is pressed go to that sever lobby
			if( m_pNetwork->ClientConnect( vecButtons[ i ]->GetButtonText() ) )
			{
				GoToLobby();
			}
		}
	}
}

void CGameManager::HandlePacket()
{
	m_pNetwork->IsServer() ? HandlePacketServer() : HandlePacketClient();	
}

void CGameManager::HandlePacketServer()
{
	// Handle input packets from the user
	switch( m_pNetwork->Recieve() )
	{
	case UP:
		{
			m_Game.ChangeSnakeDirection( m_pNetwork->GetRecentClient(), SDIR_UP );
		}
		break;

	case DOWN:
		{
			m_Game.ChangeSnakeDirection( m_pNetwork->GetRecentClient(), SDIR_DOWN );
		}
		break;

	case LEFT:
		{
			m_Game.ChangeSnakeDirection( m_pNetwork->GetRecentClient(), SDIR_LEFT );
		}
		break;

	case RIGHT:
		{
			m_Game.ChangeSnakeDirection( m_pNetwork->GetRecentClient(), SDIR_RIGHT );
		}
		break;
	case SCORES:
		{
			m_pNetwork->Send( SCORES, m_Game.GetScores() );
		}
		break;
	}
}

void CGameManager::HandlePacketClient()
{
	switch( m_pNetwork->Recieve() )
	{
	case STATE:
		{
			// If the user is in the lobby
			if( m_eMenuType == MENU_LOBB )
			{
				// Set post game to main menu
				m_eMenuType = MENU_MAIN;
				m_pCurrentMenu = &m_MenuMain;

				// Sent the board state sent from the server to initialise the game state as client
				m_Game.InitialiseAsClient( m_pTextBox->GetText(), m_vTextOffset, m_pDefaultFont, m_pNetwork->GetTextMessage() );
				m_bInGame = true;
			}
			else if( m_bInGame )
			{
				// Update the board from the server packet
				m_Game.SetBoard( m_pNetwork->GetTextMessage() );
			}
		}
		break;

	case USERS:
		{
			if( m_eMenuType == MENU_LOBB )
			{
				OutputLobbyNames();
			}
		}
		break;

	case QUIT:
		{
			// Server has quit 
			m_bInGame = false;
			m_Game.ResetGame();
			m_pNetwork->Reset();

			// Sleep to display board before moving to game over screen
			Sleep( 2000 );

			GoToMain();
		}
		break;

	case SCORES:
		{
			m_Game.UpdateText( m_pNetwork->GetTextMessage() );
		}
		break;
	}
}

void CGameManager::Ready()
{
	m_pReadyButton->SetEnabled( false );
	m_pNetwork->Send( READY, "" );
}


void CGameManager::OutputLobbyNames()
{
	// Outputs the client names in lobby screen
	std::vector< std::string > vecUsers = m_pNetwork->UserNamesClient();

	for( unsigned i = 0; i < g_SnakeGameMaxNumPlayers; ++i )
	{
		m_pLobbyConnections[ i ]->SetButtonText( i < vecUsers.size() ? vecUsers[ i ] : "" );
	}
}


void CGameManager::OutputConnectedClients()
{
	// Outputs the client names in server screen
	std::vector< TUser > vecUsers = m_pNetwork->GetClients();

	for( unsigned i = 0; i < g_SnakeGameMaxNumPlayers; ++i )
	{
		m_pServerConnections[ i ]->SetButtonText( i < vecUsers.size() ? vecUsers[ i ].t_Username : ""   );
	}
}