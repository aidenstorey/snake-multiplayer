#ifndef GUARD_AIDEN_STOREY_TEXT_BOX_H_20140616
#define GUARD_AIDEN_STOREY_TEXT_BOX_H_20140616

// Library Includes
#include <string>

// Local Includes
#include "keyboard_input.h"
#include "point_sprite.h"

// constant variables
const D3DXCOLOR		g_TextBoxColorDefault	= D3DXCOLOR( 0.5f, 0.5f, 0.5f, 1.0f );
const D3DXCOLOR		g_TextBoxColorSelected	= D3DXCOLOR( 0.6f, 0.6f, 0.6f, 1.0f );
const int			g_TextBoxMaxCharacters	= 12;
const std::string	g_TextBoxTextDefault	= "Name Here";

// Prorotype
class CTextBox : public CPointSprite
{
	// Member Functions
public:
	// Constructors
	CTextBox();
	CTextBox( const D3DXVECTOR2& _vPosition, const D3DXVECTOR2& _vScale, 
		const D3DXVECTOR2& _vTextOffset, ID3DX10Font* _pFont );
	~CTextBox();

	// Accessors
	std::string GetText() const;
	void SetText( const std::string _sText );

	bool GetSelected() const;
	void SetSelected( const bool _bSelected );

	// Other
	void Update();
	void OutputText();

	virtual bool InHitBox( const D3DXVECTOR2 _vPosition );

protected:
private:

	// Member Variables
public:
protected:
private:
	RECT			m_HitBox;
	RECT			m_TextBox;

	std::string		m_sText;
	ID3DX10Font*	m_pFont;

	bool			m_bSelected;

	CKeyboardInput*	m_pKeyboard;
};

#endif // GUARD_AIDEN_STOREY_TEXT_BOX_H_20140616