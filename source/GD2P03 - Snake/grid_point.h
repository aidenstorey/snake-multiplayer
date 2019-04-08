#ifndef GUARD_AIDEN_STOREY_GRID_POINT_H_20140619
#define GUARD_AIDEN_STOREY_GRID_POINT_H_20140619

typedef struct tagGridPoint
{
	tagGridPoint() {};
	tagGridPoint( int _x, int _y ) :
		x( _x ), y( _y ) {}

	bool operator < ( const tagGridPoint& _tPoint )
	{
		return ( x < _tPoint.x && y < _tPoint.y );
	}

	bool operator > ( const tagGridPoint& _tPoint )
	{
		return ( x > _tPoint.x && y > _tPoint.y );
	}

	bool operator <= ( const tagGridPoint& _tPoint )
	{
		return ( x <= _tPoint.x && y <= _tPoint.y );
	}

	bool operator >= ( const tagGridPoint& _tPoint )
	{
		return ( x >= _tPoint.x && y >= _tPoint.y );
	}

	bool operator == ( const tagGridPoint& _tPoint )
	{
		return ( x == _tPoint.x && y == _tPoint.y );
	}

	void Randomize( int _iMin, int _iMax )
	{
		x = GenerateRandom( _iMin, _iMax );
		y = GenerateRandom( _iMin, _iMax );
	}

	int x;
	int y;
} TGridPoint;

#endif // GUARD_AIDEN_STOREY_GRID_POINT_H_20140619