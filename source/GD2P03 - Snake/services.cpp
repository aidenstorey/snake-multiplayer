//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: services.cpp
// Description	: Stores a pointer to objects that can be checked out and used 
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

// This Include 
#include "services.h"

// Static Variables
CMouseInput* CServices::s_pMouse = nullptr;
CKeyboardInput* CServices::s_pKeyboard = nullptr;
ID3D10Device* CServices::s_pDevice = nullptr;
Network* CServices::s_pNetwork = nullptr;

// Implementation
void CServices::ProvideService( EServiceType _eService, void* _pService )
{
	switch( _eService )
	{
		case SERVICETYPE_MOUSE:
		{
			s_pMouse = static_cast< CMouseInput* >( _pService );
		}
		break;

		case SERVICETYPE_KEYBOARD:
		{
			s_pKeyboard = static_cast< CKeyboardInput* >( _pService );
		}
		break;

		case SERVICETYPE_D3DDEVICE:
		{
			s_pDevice = static_cast< ID3D10Device* >( _pService );
		}
		break;

		case SERVICETYPE_NETWORKDEVICE:
		{
			s_pNetwork = static_cast< Network* >( _pService );
		}
		break;

		default: break;
	}
}
void CServices::GetService( EServiceType _eService, void*& _rpService )
{
	switch( _eService )
	{
		case SERVICETYPE_MOUSE:
		{
			_rpService = static_cast< void* >( s_pMouse );
		}
		break;

		case SERVICETYPE_KEYBOARD:
		{
			_rpService = static_cast< void* >( s_pKeyboard );
		}
		break;

		case SERVICETYPE_D3DDEVICE:
		{
			_rpService = static_cast< void* >( s_pDevice );
		}
		break;

		case SERVICETYPE_NETWORKDEVICE:
		{
			_rpService = static_cast< void* >( s_pNetwork );
		}
		break;

		default: break;
	}
}