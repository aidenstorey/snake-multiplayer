#ifndef GUARD_AIDEN_STOREY_GAME_BOARD_H_20140619
#define GUARD_AIDEN_STOREY_GAME_BOARD_H_20140619

// Library Includes
#include <vector>

// Local Includes
#include "d3dUtil.h"
#include "grid_point.h"
#include "point_sprite.h"


// Constant Variables

const int		g_BoardStateValueInit	= 65; // To send A - Z as packet data
const int		g_BoardStateValueOffset	= 3; // To send offset for extra non-player values
const int		g_BoardStateDeadSnake	= -3;
const int		g_BoardStateFruit		= -2;
const int		g_BoardStateEmpty		= -1;

const D3DXCOLOR g_BoardColorSnakes [16] = 
{
	D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ),
	D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ),
	D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f ),
	
	D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ),
	D3DXCOLOR( 1.0f, 0.0f, 1.0f, 1.0f ),
	D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ),

	D3DXCOLOR( 0.6f, 0.0f, 0.0f, 1.0f ),
	D3DXCOLOR( 0.0f, 0.6f, 0.0f, 1.0f ),
	D3DXCOLOR( 0.0f, 0.0f, 0.6f, 1.0f ),
	
	D3DXCOLOR( 0.0f, 0.6f, 0.6f, 1.0f ),
	D3DXCOLOR( 0.6f, 0.0f, 0.6f, 1.0f ),
	D3DXCOLOR( 0.6f, 0.6f, 0.0f, 1.0f ),

	D3DXCOLOR( 0.2f, 0.0f, 0.0f, 1.0f ),
	D3DXCOLOR( 0.0f, 0.2f, 0.0f, 1.0f ),
	D3DXCOLOR( 0.0f, 0.0f, 0.2f, 1.0f ),
	D3DXCOLOR( 0.2f, 0.0f, 0.2f, 1.0f ),
};

const D3DXCOLOR g_BoardColorEmpty = D3DXCOLOR( 0.2f, 0.2f, 0.2f, 1.0f );
const D3DXCOLOR g_BoardColorSnakeDead = D3DXCOLOR( 0.4f, 0.4f, 0.4f, 1.0f );
const D3DXCOLOR g_BoardColorFruit = D3DXCOLOR( 1.0f, 0.2f, 0.2f, 1.0f );

// Prototypes
class CGameBoard
{
	// Memeber Fucntions
public:
	// Constructors
	CGameBoard();
	~CGameBoard();

	// Accessors
	int GetUnitState( TGridPoint _tPosition ) const;
	void SetUnitState( TGridPoint _tPosition, int _iState );

	TGridPoint GetSize() const;

	D3DXVECTOR2 GetAnchor() const;
	void SetAnchor( const D3DXVECTOR2 _vAnchor );

	D3DXVECTOR2 GetSeperation() const;
	void SetSeperation( const D3DXVECTOR2 _vSeperation );

	D3DXVECTOR2 GetScale() const;
	void SetScale( const D3DXVECTOR2 _vScale );

	TGridPoint GetFruitPosition() const;
	void SetFruitPosition( const TGridPoint _tPosition );

	std::string GetStateAsString() const;
	void SetStateFromString( const std::string _State );

	// Other
	void InitialiseAsClient( const TGridPoint _tBoardSize, const D3DXVECTOR2 _vAnchor, const D3DXVECTOR2 _vSeperation, const D3DXVECTOR2 _vScale  );
	void InitialiseAsServer( const TGridPoint _tBoardSize );

	bool IsOnBoard( TGridPoint _tPosition ) const;
	bool CanMove( TGridPoint _tPosition ) const;

	void ResetFruit();

	bool HasFruitReset();

	void Update();
	void Draw();

protected:
	void MapBuffer();

	char StateToValue( int _iState ) const;
	int ValueToState( char _Value ) const;

	D3DXCOLOR StateColor( const int _iState );
private:

	// Member Variables
public:
protected:
private:
	ID3D10Device*					m_pDevice;
	ID3D10Buffer*					m_pVertexBuffer;

	TGridPoint						m_tBoardSize;
	D3DXVECTOR2						m_vBoardAnchor;
	D3DXVECTOR2						m_vBoardSeperation;
	D3DXVECTOR2						m_vBoardScale;

	TGridPoint						m_tFruitPosition;

	int**							m_iBoard;

	std::vector< CPointSprite >	m_vecVertices;

	bool							m_bFruitReset;
};

#endif // GUARD_AIDEN_STOREY_GAME_BOARD_H_20140619