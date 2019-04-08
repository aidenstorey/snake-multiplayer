#ifndef GUARD_AIDEN_STOREY_SNAKE_GAME_H_20140616
#define GUARD_AIDEN_STOREY_SNAKE_GAME_H_20140616

// Library Includes
#include <vector>

// Local Includes
#include "game_board.h"
#include "keyboard_input.h"
#include "Network.h"
#include "player_snake.h"
#include "point_sprite.h"
#include "static_text.h"

// Constant Variables
const TGridPoint	g_SnakeGameBoardSize		= TGridPoint( 30, 30 );
const D3DXVECTOR2	g_SnakeGameBoardAnchor		= D3DXVECTOR2( -168.0f, 100.0f );
const D3DXVECTOR2	g_SnakeGameBoardSeperation	= D3DXVECTOR2( 8.0f, 8.0f );
const D3DXVECTOR2	g_SnakeGameScale			= D3DXVECTOR2( 6.0f, 6.0f );
const D3DXVECTOR2	g_SnakeGameScoresAnchor		= D3DXVECTOR2( 147.0f, 135.0f );
const unsigned		g_SnakeGameMaxNumPlayers	= 16;
const int			g_SnakeGameInitialLength	= 3;

// Prototype
class CSnakeGame
{
	// Member Fucntions
public:
	// Constructors
	CSnakeGame();
	~CSnakeGame();

	// Accessors
	std::string GetName() const;
	void SetName( const std::string _Name );

	std::string GetScores() const;

	void ChangeSnakeDirection( unsigned _SnakeNum, ESnakeDirection _eDirection );

	// Other
	void InitialiseAsClient( std::string _Name, D3DXVECTOR2 _vTextOffset, ID3DX10Font* _pFont, std::string _BoardState );
	void InitialiseAsServer();
	void Update( float _fDT );
	void Draw();

	void SetBoard( std::string _boardState );
	void UpdateText( std::string _scores );
	void ResetGame();
	bool IsOver();

protected:
	void InitialiseText();

	void MapBuffer();
	void CheckKeyboard();

private:

	// Member Variables
public:
protected:
private:
	ID3D10Device*					m_pDevice;
	ID3D10Buffer*					m_pVertexBuffer;
	ID3DX10Font*					m_pFont;

	CKeyboardInput*					m_pKeyboard;

	int								m_iNumPlayers;

	float							m_fTimePassed;

	std::vector< CPointSprite* >	m_vecVertices;

	CGameBoard*						m_pGameBoard;

	Network*						m_pNetwork;

	CStaticText*					m_pPlayers[ g_SnakeGameMaxNumPlayers ];
	CStaticText*					m_pYourScore;
	CStaticText*					m_pHighScore;

	D3DXVECTOR2						m_vTextOffset;

	std::string						m_Name;
	std::vector< CPlayerSnake >		m_Snakes;

	bool							m_bServer;
};

#endif // GUARD_AIDEN_STOREY_SNAKE_GAME_H_20140616