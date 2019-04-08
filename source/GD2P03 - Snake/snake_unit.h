#ifndef GUARD_AIDEN_STOREY_SNAKE_UNIT_H_20140617
#define GUARD_AIDEN_STOREY_SNAKE_UNIT_H_20140617

// Local Includes
#include "point_sprite.h"

// Prototype
class CSnakeUnit : public CPointSprite
{
	// Member Functions
public:
	// Constructors
	CSnakeUnit();
	CSnakeUnit( const D3DXVECTOR2& _vPosition, const D3DXVECTOR2& _vScale, const D3DXCOLOR& _vColor );
	~CSnakeUnit();

	// Accessors

	// Other
	virtual bool InHitBox( const D3DXVECTOR2 _vPosition );
protected:
private:

	// Member Variables
public:
protected:
private:
};

#endif // GUARD_AIDEN_STOREY_SNAKE_UNIT_H_20140617