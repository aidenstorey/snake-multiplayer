#ifndef GUARD_AIDEN_STOREY_KEYBOARD_INPUT_H_20140603
#define GUARD_AIDEN_STOREY_KEYBOARD_INPUT_H_20140603

// Constant Variablies
const int g_kiKeyboardMaxKeys = 256;

// Prototypes
class CKeyboardInput
{
	// Member Functions
public:
	// Constructors
	CKeyboardInput();
	~CKeyboardInput();

	// Other
	bool KeyPressed( const unsigned _uiMsg );
	bool KeyDown( const unsigned _uiMsg );
	bool KeyReleased( const unsigned _uiMsg );

	void UpdateKey( const unsigned _uiMsg, const bool _bKeyPressed );
	void UpdatePrevious();

protected:
private:

	// Member Variables
public:
protected:
private:
	bool m_bCurrKeys[ g_kiKeyboardMaxKeys ];
	bool m_bPrevKeys[ g_kiKeyboardMaxKeys ];
};

#endif // GUARD_AIDEN_STOREY_KEYBOARD_INPUT_H_20140603