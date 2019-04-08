#ifndef GUARD_AIDEN_STOREY_MOUSE_INPUT_H_20140530
#define GUARD_AIDEN_STOREY_MOUSE_INPUT_H_20140530

// Local Includes
#include "d3dUtil.h"

// Constant variables
const int g_kiMouseMaxButtons = 3;

// Prototypes
class CMouseInput
{
	// Member Functions
public:
	// Constructors
	CMouseInput();
	~CMouseInput();

	// Accessors
	D3DXVECTOR2 GetPosition() const;

	HWND GetHWND() const;
	void SetHWND( const HWND _hWnd );

	void SetWindowSize( const int _kiWidth, const int _kiHeight );

	// Queries
	bool LeftPressed() const;
	bool LeftDown() const;
	bool LeftReleased() const;

	bool MiddlePressed() const;
	bool MiddleDown() const;
	bool MiddleReleased() const;

	bool RightPressed() const;
	bool RightDown() const;
	bool RightReleased() const;

	// Other
	void ButtonDown( unsigned int _uiMsg );
	void ButtonUp( unsigned int _uiMsg );
	void Update();

protected:
private:

	// Member Variables
public:
protected:
	HWND		m_hWnd;
	int			m_iWidth;
	int			m_iHeight;

	D3DXVECTOR2	m_Position;

	bool		m_bButtonCurr[ g_kiMouseMaxButtons ];
	bool		m_bButtonPrev[ g_kiMouseMaxButtons ];
private:

};

#endif // GUARD_AIDEN_STOREY_MOUSE_INPUT_H_20140530