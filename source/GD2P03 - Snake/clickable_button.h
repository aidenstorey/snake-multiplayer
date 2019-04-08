#ifndef GUARD_AIDEN_STOREY_CLICKABLE_BUTTON_H_20140616
#define GUARD_AIDEN_STOREY_CLICKABLE_BUTTON_H_20140616

// Library Includes
#include <string>

// Local Includes
#include "point_sprite.h"

// Color constants
const D3DXCOLOR g_ButtonColorDefault	= D3DXCOLOR( 0.3f, 0.3f, 0.3f, 1.0f );
const D3DXCOLOR g_ButtonColorHover		= D3DXCOLOR( 0.4f, 0.4f, 0.4f, 1.0f );
const D3DXCOLOR g_ButtonColorDisabled	= D3DXCOLOR( 0.1f, 0.1f, 0.1f, 1.0f );

// Prototype
class CGameManager;

class CButton : public CPointSprite
{
	// Member Fucntions
public:
	// Constructor
	CButton();
	CButton( const D3DXVECTOR2& _vPosition, const D3DXVECTOR2& _vScale, const std::string _sText, 
		const D3DXVECTOR2& _vTextOffset, ID3DX10Font* _pFont, CGameManager* _pInvoker, void ( CGameManager::*_pPressedFunc ) (), bool _bEnabled );
	~CButton();

	// Accessors
	std::string GetButtonText() const;
	void SetButtonText( const std::string _sText );

	bool GetEnabled() const;
	void SetEnabled( const bool _bEnable );
	
	void SetPressedFunction( CGameManager* _pInvoker, void ( CGameManager::*_pPressedFunc ) () );

	// Other
	void OutputText();
	void ExecuteFunction();

	bool IsBeingPressed();

	virtual bool InHitBox( const D3DXVECTOR2 _vPosition ); 
protected:
private:

	// Member variables
public:
protected:
private:
	CGameManager*	m_pInvoker;
	void ( CGameManager::*m_pPressedFunc ) ();

	RECT			m_HitBox;
	RECT			m_TextBox;
	std::string		m_sText;

	ID3DX10Font*	m_pFont; 

	bool			m_bEnabled;
	bool			m_bBeingPressed;
};

#endif // GUARD_AIDEN_STOREY_CLICKABLE_BUTTON_H_20140616