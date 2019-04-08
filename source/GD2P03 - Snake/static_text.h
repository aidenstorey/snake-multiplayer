#ifndef GUARD_AIDEN_STOREY_STATIC_TEXT_H_20140616
#define GUARD_AIDEN_STOREY_STATIC_TEXT_H_20140616

// Library Includces
#include <string>

// Local Includes
#include "point_sprite.h"

// Prototype
class CStaticText : public CPointSprite
{
	// Member Functions
public:
	// Constructors
	CStaticText();
	CStaticText( const D3DXVECTOR2& _vPosition, const D3DXVECTOR2& _vScale, const std::string _sText, 
		const D3DXVECTOR2& _vTextOffset, ID3DX10Font* _pFont, D3DXCOLOR _vBackgroundColor );
	~CStaticText();

	// Accessors
	std::string GetButtonText() const;
	void SetButtonText( const std::string _sText );

	// Other
	void OutputText();
	
	virtual bool InHitBox( const D3DXVECTOR2 _vPosition );

protected:
private:

	// Member Variables
public:
protected:
private:
	RECT			m_TextBox;
	std::string		m_sText;
	
	ID3DX10Font*	m_pFont;
};

#endif // GUARD_AIDEN_STOREY_STATIC_TEXT_H_20140616