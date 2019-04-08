#ifndef GUARD_AIDEN_STOREY_MENU_H_20140616
#define GUARD_AIDEN_STOREY_MENU_H_20140616

// Library Includes
#include <vector>

// Local Includes
#include "clickable_button.h"
#include "d3dUtil.h"
#include "keyboard_input.h"
#include "mouse_input.h"
#include "point_sprite.h"
#include "static_text.h"
#include "text_box.h"

// Prototype
class CMenu
{
	// Member Fucntions
public:
	// Constructor
	CMenu();
	~CMenu();

	// Accessors
	std::vector< CButton* >& GetButtons();

	// Other
	void Initialise();

	void Update( float _fDT );
	void Draw();

	void AddButton( CButton* _pButton );
	void AddText( CStaticText* _pText );
	void AddTextBox( CTextBox* _pTextBox );

protected:
	void InitialiseBuffer();
	void MapBuffer();

private:

	// Member variables
public:
protected:
private:
	std::vector< CButton* >			m_vecButtons;
	std::vector< CPointSprite* >	m_vecVertices;
	std::vector< CStaticText* >		m_vecStaticText;
	std::vector< CTextBox* >		m_vecTextBox;

	ID3D10Device*					m_pDevice;
	ID3D10Buffer*					m_pVertexBuffer;

	CMouseInput*					m_pMouse;
	CKeyboardInput*					m_pKeyboard;
};

#endif // GUARD_AIDEN_STOREY_MENU_H_20140616