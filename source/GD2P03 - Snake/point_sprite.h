#ifndef GUARD_AIDEN_STOREY_POINT_SPRITE_H_20140601
#define GUARD_AIDEN_STOREY_POINT_SPRITE_H_20140601

// Local Includes
#include "d3dUtil.h"

// Prototype
class CPointSprite
{
public:
	// Constructors
	CPointSprite();
	CPointSprite( const D3DXVECTOR2& _vPosition, const D3DXVECTOR2& _vScale, 
		const float& _fRotation, const D3DXCOLOR& _vColor );
	virtual ~CPointSprite();

	// Accessors
	D3DXVECTOR2 GetPosition() const;
	void SetPosition( const D3DXVECTOR2& _vPosition );

	D3DXVECTOR2 GetScale() const;
	void SetScale( const D3DXVECTOR2& _vScale );

	float GetRotation() const;
	void SetRotation( const float _fRotation );

	D3DXCOLOR GetColor() const;
	void SetColor( const D3DXCOLOR _vColor );

	// Other
	virtual bool InHitBox( const D3DXVECTOR2 _vPosition );
	
protected:
private:
		
	// Member Variables
public:
protected:
	D3DXVECTOR2 m_vPosition;
	D3DXVECTOR2 m_vScale;
	float		m_fRotation;
	D3DXCOLOR	m_vColor;

private:
};

#endif // GUARD_AIDEN_STOREY_POINT_SPRITE_H_20140601