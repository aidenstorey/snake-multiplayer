#include "Network.h"

Network::Network()
{
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);
	m_Operator = new Operator();
}

Network::~Network()
{
	delete m_Operator;
}

int Network::GetRecentClient()
{
	int iReturn = -1;

	for( unsigned i = 0; i < m_Clients->size(); i++)
	{
		if ((strcmp(( *m_Clients )[i].t_Address.c_str(), m_CurrentPacket.t_Address.c_str()) == 0) && (strcmp(( *m_Clients )[i].t_Port.c_str(), m_CurrentPacket.t_Port.c_str()) == 0))
		{
			iReturn = i;
		}
	}

	return iReturn;
}

/* Call this to setup the Operator with a name */
bool Network::Setup(std::string _sName, bool _bServer)
{
	bool bReturn = false;

	if (_bServer)
	{
		bReturn = m_Operator->SetServer(_sName);
	}
	else
	{
		bReturn = m_Operator->SetClient(_sName);
	}

	return bReturn;
}

/* Call this to broadcast and recieve a string of server IPs */
std::vector<std::string> Network::Broadcast()
{
	m_Servers.clear();
	std::vector<std::string> sServers;;

	m_Servers = m_Operator->ClientBroadcast();

	for (unsigned i = 0; i < m_Servers.size(); i++)
	{
		sServers.push_back(m_Servers[i].t_Username);
	}

	return sServers;
}



/* Retrieve a vector of clients */
std::vector<TUser>& Network::GetClients()
{
	return m_Operator->RetrieveUsers();
}

/* Connect the client to the passed in _ip, returns a sucess*/
bool Network::ClientConnect(std::string _ip)
{
	std::string IPADDRESS;
	for ( unsigned i = 0; i < m_Servers.size(); i++)
	{
		if (strcmp(_ip.c_str(), m_Servers[i].t_Username.c_str()) == 0)
		{
			IPADDRESS = m_Servers[i].t_Address;
		}
	}
	return m_Operator->ClientConnect(IPADDRESS);
}

/* Call this to get the Current state of the Current Packet*/
std::string Network::GetTextMessage()
{
	std::string sState = "";

	sState = m_CurrentPacket.t_Message;

	return sState;
}

/* Call this to send a message */
void Network::Send(ECOMMAND _Message, std::string _sState)
{
	std::string sMessage = "";

	if (m_Operator->IsServer())
	{
		switch (_Message)
		{
		case STATE:
			sMessage = _sState;
			m_Operator->RelayMessage(STATE_MESSAGE, sMessage);
			break;
		case SCORES:
			sMessage = _sState;
			m_Operator->RelayMessage(SCORE_MESSAGE, sMessage);
			break;
		case READY:
			sMessage = _sState;
			m_Operator->RelayMessage(READY_MESSAGE, sMessage);
			break;
		case USERS:
			m_Operator->RelayMessage(DISPLAY_USERS, GetUserNames());
			break;
		case QUIT:
			sMessage = _sState;
			m_Operator->RelayMessage(SEND_MESSAGE, sMessage);
			break;
		}
	}
	else
	{
		switch (_Message)
		{
		case UP:
			sMessage = "UP";
			break;
		case DOWN:
			sMessage = "DOWN";
			break;
		case LEFT:
			sMessage = "LEFT";
			break;
		case RIGHT:
			sMessage = "RIGHT";
			break;
		case SCORES:
			sMessage = "SCORES";
			break;
		case READY:
			sMessage = "READY";
			break;
		case USERS:
			sMessage = "USERS";
			break;
		case QUIT:
			sMessage = "QUIT";
			break;
		}
		m_Operator->SendClientMessage(SEND_MESSAGE, sMessage);
	}
}

/* Call this to recieve a message */
ECOMMAND Network::Recieve()
{
	Run();
	ECOMMAND eReturn = EMPTY;

	if (!m_Operator->IsServer())
	{
		if (m_CurrentPacket.t_MessageType == STATE_MESSAGE)
		{
			eReturn = STATE;
		}
		else if (m_CurrentPacket.t_MessageType == SCORE_MESSAGE)
		{
			eReturn = SCORES;
		}
		else if (m_CurrentPacket.t_MessageType == READY_MESSAGE)
		{
			eReturn = READY;
		}
		else if (m_CurrentPacket.t_MessageType == DISPLAY_USERS)
		{
			eReturn = USERS;
		}
		else if (m_CurrentPacket.t_MessageType == SEND_MESSAGE)
		{
			if(std::strcmp(m_CurrentPacket.t_Message.c_str(), "USERS") == 0)
			{
				eReturn = QUIT;
			}
		}
		
	}
	else
	{
		if (m_CurrentPacket.t_MessageType == SEND_MESSAGE)
		{
			if (std::strcmp(m_CurrentPacket.t_Message.c_str(), "UP") == 0)
			{
				eReturn = UP;
			}
			else if (std::strcmp(m_CurrentPacket.t_Message.c_str(), "DOWN") == 0)
			{
				eReturn = DOWN;
			}
			else if (std::strcmp(m_CurrentPacket.t_Message.c_str(), "LEFT") == 0)
			{
				eReturn = LEFT;
			}
			else if (std::strcmp(m_CurrentPacket.t_Message.c_str(), "RIGHT") == 0)
			{
				eReturn = RIGHT;
			}
			else if (std::strcmp(m_CurrentPacket.t_Message.c_str(), "SCORES") == 0)
			{
				eReturn = SCORES;
			}
			else if (std::strcmp(m_CurrentPacket.t_Message.c_str(), "READY") == 0)
			{
				eReturn = READY;
				ReadyClient( m_CurrentPacket );
			}
			else if (std::strcmp(m_CurrentPacket.t_Message.c_str(), "QUIT") == 0)
			{
				eReturn = QUIT;
			}
			else if (std::strcmp(m_CurrentPacket.t_Message.c_str(), "USERS") == 0)
			{
				eReturn = USERS;
				Send(USERS, "");
			}
		}
	}

	return eReturn;
}

void Network::Reset()
{
	delete m_Operator;
	m_Operator = new Operator();
	m_Servers.clear();
	m_CurrentPacket.Clear();
}

/* Call after client has recieved a USER message */
std::vector<std::string> Network::UserNamesClient()
{
	std::vector<std::string> Names;
	
	std::string sMessage = m_CurrentPacket.t_Message;
	std::string sTemp = "";

	unsigned iIndex = 0;

	while (iIndex < sMessage.length())
	{
		if (sMessage.c_str()[iIndex] == ',')
		{
			Names.push_back(sTemp);
			sTemp = "";
			iIndex++;
		}
		else
		{
			sTemp += sMessage.c_str()[iIndex];
			iIndex++;
		}
	}
	return Names;
}

bool Network::IsServer()
{
	return m_Operator->IsServer();
}

bool Network::CheckClientsReady()
{
	m_Clients = &GetClients();
	bool Return = true;
	for (unsigned i = 0; i < m_Clients->size(); i++)
	{
		if (( *m_Clients )[i].t_Ready == false)
		{
			Return = false;
		}
	}
	return (Return && (m_Clients->size() >= 2));
}

bool Network::IsGameReady()
{
	bool bReturn = CheckClientsReady();

	if(bReturn)
	{
		m_Operator->SetRunningState(true);
	}

	return(bReturn);
}

void Network::ReadyClient(TUser& _Packet)
{
	for (unsigned i = 0; i < m_Clients->size(); i++)
	{
		if ((strcmp(( *m_Clients )[i].t_Address.c_str(), _Packet.t_Address.c_str()) == 0) && (strcmp(( *m_Clients )[i].t_Port.c_str(), _Packet.t_Port.c_str()) == 0))
		{
			( *m_Clients )[i].t_Ready = true;
		}
	}
}

bool Network::IsGameRunning()	
{
	return m_Operator->IsRunning();
}

//////////////////////// PRIVATE ///////////////////////

/* Private Message to Run */
void Network::Run()
{
	m_CurrentPacket.Clear();
	m_CurrentPacket = m_Operator->Run();
}

/* Just for Testing */
std::string Network::GetStr()
{
	return m_CurrentPacket.t_Message;
}

/* Retrieves UserNames*/
std::string Network::GetUserNames()
{
	return m_Operator->GetUsers();
}