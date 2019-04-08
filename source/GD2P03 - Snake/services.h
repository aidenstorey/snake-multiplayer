#ifndef GUARD_AIDEN_STOREY_SERVICES_H_20140531
#define GUARD_AIDEN_STOREY_SERVICES_H_20140531

// Local Includes
#include "d3dApp.h"
#include "d3dUtil.h"
#include "mouse_input.h"
#include "Network.h"
#include "keyboard_input.h"

// Prototypes
enum EServiceType
{
	SERVICETYPE_MOUSE,
	SERVICETYPE_KEYBOARD,
	SERVICETYPE_D3DDEVICE,
	SERVICETYPE_NETWORKDEVICE,
};


class CServices
{
	// Member Functions
public:
	// Accessors
	static void ProvideService( EServiceType _eService, void* _pService );
	static void GetService( EServiceType _eService, void*& _rpService );

protected:
private:

	// Member Variables
public:
protected:
private:
	static CMouseInput*		s_pMouse;
	static CKeyboardInput*	s_pKeyboard;
	static ID3D10Device*	s_pDevice;
	static Network*			s_pNetwork;
};

#endif // GUARD_AIDEN_STOREY_SERVICES_H_20140531