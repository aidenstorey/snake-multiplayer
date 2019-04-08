#ifndef GUARD_AIDEN_STOREY_GAME_MANAGER_H_20140616
#define GUARD_AIDEN_STOREY_GAME_MANAGER_H_20140616

// Library Includes
#include <vector>

// Local Includes
#include "clickable_button.h"
#include "menu.h"
#include "Network.h"
#include "snake_game.h"
#include "text_box.h"

// Enumerations
enum EMenuType
{
	MENU_MAIN,
	MENU_FIND,
	MENU_LOBB,
	MENU_INST,
	MENU_SERV,
	MENU_OVER,

	MENU_NONE,
};

// Prototype
class CGameManager
{
	// Member Functions
public:
	// Constructors
	CGameManager();
	~CGameManager();

	// Other
	void Initialise( int _iClientWidth, int _iClientHeight );
	void Update( float _fDT );
	void Draw();

protected:	
	void InitialiseFonts();
	void InitialiseMenues();
	void InitialiseLobbyText();
	void InitialiseSeverText();

	void GoToMain();
	void GoToInstructions();
	void GoToFindServer();
	void GoToLobby();
	void GoToSever();

	void QuitApplication();
	void ConnectToServer();
	void Ready();
	void OutputLobbyNames();
	void OutputConnectedClients();

	void HandlePacket();
	void HandlePacketServer();
	void HandlePacketClient();

private:

	// Member Variables
public:
protected:
private:
	ID3DX10Font*	m_pTitleFont;
	ID3DX10Font*	m_pDefaultFont;

	CSnakeGame		m_Game;

	CMenu			m_MenuMain;
	CMenu			m_MenuInstructions;
	CMenu			m_MenuFindServer;
	CMenu			m_MenuLobby;
	CMenu			m_MenuServer;
	CMenu			m_MenuOver;

	CMenu*			m_pCurrentMenu;

	bool			m_bInGame;

	float			m_fClientWidth;
	float			m_fClientHeight;

	Network*		m_pNetwork;
	EMenuType		m_eMenuType;

	CTextBox*		m_pTextBox;
	CButton*		m_pFindButton;
	CButton*		m_pHostButton;
	CButton*		m_pReadyButton;

	CStaticText*	m_pLobbyConnections[ g_SnakeGameMaxNumPlayers ];
	CStaticText*	m_pServerConnections[ g_SnakeGameMaxNumPlayers ];

	D3DXVECTOR2		m_vTextOffset;
};

#endif // GUARD_AIDEN_STOREY_GAME_MANAGER_H_20140616