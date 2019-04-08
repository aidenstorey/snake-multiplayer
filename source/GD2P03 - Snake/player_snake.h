#ifndef GUARD_AIDEN_STOREY_PLAYER_SNAKE_H_20140619
#define GUARD_AIDEN_STOREY_PLAYER_SNAKE_H_20140619

// Library Includes
#include <vector>

// Local Includes
#include "d3dUtil.h"
#include "game_board.h"
#include "grid_point.h"

// Enumerations
enum ESnakeDirection
{
	SDIR_UP,
	SDIR_DOWN,
	SDIR_LEFT,
	SDIR_RIGHT,

	SDIR_MAX,
	SDIR_NONE,
};

enum ESnakeType
{
	SNAKETYPE_PLAYER,
	SNAKETYPE_ENEMY,
};

// Prototype
class CPlayerSnake
{
	// Member Functions
public:
	// Constructors
	CPlayerSnake();
	~CPlayerSnake();

	// Accessors
	int GetPlayerNum() const;
	void SetPlayerNum( const int _iPlayerNum );

	bool GetAlive() const;
	void SetAlive( const bool _bAlive );

	ESnakeDirection GetDirection() const;
	void SetDirection( const ESnakeDirection _eDiection );

	unsigned GetLength() const;

	std::string GetName() const;
	void SetName( const std::string _Name );

	// Other
	void Initialise( std::string _Name, CGameBoard* _pGameBoard, const TGridPoint _StartPoint, const int _iStartLength, const ESnakeDirection _eDirection, const int _iPlayerNum );
	void ChangeDirection( const ESnakeDirection _eDirection );
	void AddUnit();
	void Move();
protected:
private:

	// Member Variables
public:
protected:
private:
	CGameBoard*					m_pGameBoard;
	int							m_iPlayerNum;

	bool						m_bAlive;
	ESnakeDirection				m_eDirection;
	std::vector< TGridPoint >	m_vecPosition;

	std::string					m_Name;

	bool						m_bCanChange;
};

#endif // GUARD_AIDEN_STOREY_PLAYER_SNAKE_H_20140619