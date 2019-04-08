#include "Operator.h"

Operator::Operator()
{
	m_pTimeVal = NULL;
	m_tv.tv_sec = 0;
	m_tv.tv_usec = iNET_SLEEP;
	m_ClientConnected = false;
	m_bServerCheck = false;

	m_MaxTime = 20;
	m_CurrentTime = 0;

	/* Update Time */
	m_pTimeVal = &m_tv;

	m_uCLIENT_PORT = 60013;
	m_uSERVER_PORT = 60012;
	m_uBROADCAST_PORT = 60015;
	m_iMessageNum = 0;
	m_GameRunning = false;
}

bool Operator::IsRunning()
{
	return m_GameRunning;
}

void Operator::SetRunningState(bool _t)
{
	m_GameRunning = _t;
}

Operator::~Operator()
{
	closesocket( m_Socket );
}

bool Operator::SetServer(std::string _name)
{
	m_OperatorData.t_Username = _name;

	m_bServer = true;

	/* Get Local IP */
	char name[255];
	char* ip;
	gethostname(name, sizeof(name));
	PHOSTENT hostinfo = gethostbyname(name);
	ip = inet_ntoa (*(struct in_addr *)hostinfo->h_addr_list[0]);

	m_OperatorData.t_Address = ip;

	/* Sets the Ports for the server */
	m_uSERVER_PORT = uDEFAULT_SERVER_PORT;

	/* Sets the Address for the Current Operator */
	SetCurrentAddress();
	m_sCurrentAddress.sin_port = htons(m_uSERVER_PORT);
	
	/* Bind the Socket with the Address Port Number */
	BindSocket();

	if (m_Socket != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Operator::SetClient(std::string _name)
{
	m_OperatorData.t_Username = _name;

	m_bServer = false;

	/* Get Local IP */
	char name[255];
	char* ip;
	gethostname(name, sizeof(name));
	PHOSTENT hostinfo = gethostbyname(name);
	ip = inet_ntoa(*(struct in_addr *)hostinfo->h_addr_list[0]);

	m_OperatorData.t_Address = ip;

	/* Sets the Ports for the server */
	m_uSERVER_PORT = uDEFAULT_SERVER_PORT;

	/* Sets the Address for the Current Operator */
	SetCurrentAddress();
	m_sCurrentAddress.sin_port = htons(m_uCLIENT_PORT);

	/* Bind the Socket with the Address Port Number */
	BindSocket();

	if (m_Socket != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::vector<TUser>& Operator::ClientBroadcast()
{
	BroadcastSocket();

	WaitForMessages();

	return m_vClientSocketAddresses;
}

bool Operator::ClientConnect(std::string _ip)
{
	/* Sets the Server Address for the client to send to */
	memset(&m_sServerAddress, 0, sizeof(m_sServerAddress));
	m_sServerAddress.sin_family = AF_INET;
	m_sServerAddress.sin_addr.S_un.S_addr = inet_addr(_ip.c_str());
	m_sServerAddress.sin_port = htons(m_uSERVER_PORT);

	/* Send Initial Connection Packet */
	m_OperatorData.t_MessageType = CONNECTION_REQUEST;

	//BindSocket();

	SendMessage(m_OperatorData.ToPacket(), m_sServerAddress);

	while (m_ClientConnected != true)	// ADD specific wait time
	{
		WaitForMessages();
	}

	return m_ClientConnected;
}


void Operator::WaitForMessages()
{
	float fWaitTime = 0.0f;
	FD_SET ReadDesc;
	FD_SET ErrorDesc;

	while(fWaitTime < 1)
	{
		/* Set Variables */
		FD_ZERO(&ErrorDesc );
		FD_SET(m_Socket, &ErrorDesc);
		FD_ZERO(&ReadDesc );
		FD_SET(m_Socket, &ReadDesc);

		/* Wait for input */
		m_iReady = select(1, &ReadDesc, NULL, &ErrorDesc, m_pTimeVal);	

		/* Operator Recieve Message */
		if(FD_ISSET(m_Socket, &ReadDesc)) 
		{ 
			RecieveMessage();
			HandlePacket();
		}
		m_TTimer.tick();
		fWaitTime += m_TTimer.getDeltaTime();
	}
}

void Operator::HandleBroadcast()
{
	std::string Text = "\n";
	std::string Selection = "";
	for(unsigned int i = 0; i < m_vClientSocketAddresses.size(); i++)
	{
		Text += "\n";
		Text += std::to_string((long double)(i));
		Selection += std::to_string((long double)(i));
		Text += ": ";
		Text += m_vClientSocketAddresses[i].t_Username;
		Text += " - <";
		Text += m_vClientSocketAddresses[i].t_Address;
		Text += " : ";
		Text += m_vClientSocketAddresses[i].t_Port;
		Text += " >\n";
	}

	m_vClientSocketAddresses.clear();

	/*Send Connection Message*/
	m_OperatorData.t_MessageType = CONNECTION_REQUEST;
	SendMessage(m_OperatorData.ToPacket(), m_sServerAddress);
}

void Operator::SetCurrentAddress()
{
	memset(&m_sCurrentAddress, 0, sizeof( m_sCurrentAddress));
	m_sCurrentAddress.sin_family = AF_INET;
	m_sCurrentAddress.sin_addr.S_un.S_addr = INADDR_ANY;
}

void Operator::BroadcastSocket()
{
	bool bEnabled = true;

	/* Sets the ports for the Client */
	m_uSERVER_PORT = uDEFAULT_SERVER_PORT;
	m_uCLIENT_PORT = uDEFAULT_CLIENT_PORT;

	/* Sets the Address for the Current Operator */
	SetCurrentAddress();
	m_sCurrentAddress.sin_port = htons(m_uCLIENT_PORT);

	/* Bind the Socket with the Address Port Number */
	BindSocket();

	setsockopt(m_Socket, SOL_SOCKET, SO_BROADCAST, (char*)&bEnabled, sizeof(BOOL));

	/* Sets the Server Address for the client to send to */
	memset( &m_sServerAddress, 0, sizeof(m_sServerAddress));
	m_sServerAddress.sin_family = AF_INET;
	m_sServerAddress.sin_addr.S_un.S_addr = INADDR_BROADCAST;
	m_sServerAddress.sin_port=htons(m_uSERVER_PORT);

	/* Send Initial Connection Packet */
	m_OperatorData.t_MessageType = BROADCAST_REQUEST;

	/*Sends packet to passed address */
	SendMessage(m_OperatorData.ToPacket(), m_sServerAddress);
}

void Operator::BindSocket()
{
	/* Create Socket */
	m_Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == m_Socket)
	{
		//printf( "unable to create socket\n" );
		assert( false );
	}

	/* Bind Socket */
	int iRange = 0;
	while(0 != bind(m_Socket, reinterpret_cast<sockaddr*>(&m_sCurrentAddress), sizeof(m_sCurrentAddress)))
	{
		if(iRange >= 50)
		{
			/* Break if range > 20*/
			assert(false);
		}
		/* If Server then set Else Client */
		m_bServer ? m_sCurrentAddress.sin_port = htons(m_uSERVER_PORT+iRange) : m_sCurrentAddress.sin_port = htons(m_uCLIENT_PORT+iRange);
		iRange++;
	}
	/* If Server then print Else Client */

	/* Add Client buffer to Operator data*/
	if(!m_bServer)
	{
		char buffer[33];
		_itoa_s(int(m_uCLIENT_PORT+(iRange)), buffer, 10);
		m_OperatorData.t_Port = buffer;
	}
	else
	{
		char buffer[33];
		_itoa_s(int(m_uSERVER_PORT+(iRange)), buffer, 10);
		m_OperatorData.t_Port = buffer;
	}
}

void Operator::HandlePacket()
{
	if(m_bServer)
	{
		ServerHandler();
	}
	else
	{
		ClientHandler();
	}
}

void Operator::ServerHandler()
{
	// Find current 
	bool bFound = false;
	for(unsigned int i = 0; i < m_vClientSocketAddresses.size(); i++)
	{
		if(strcmp(m_vClientSocketAddresses[i].t_Address.c_str(), m_RecievedPacketData.t_Address.c_str()) == 0
			&& strcmp(m_vClientSocketAddresses[i].t_Port.c_str(), m_RecievedPacketData.t_Port.c_str()) == 0)
		{
			bFound = true;
			m_vClientSocketAddresses[i].t_AliveTime = 0;
			m_vClientSocketAddresses[i].t_bCheck = false;
		}
	}

	if(bFound)
	{
		switch((int)m_RecievedPacketData.t_MessageType)
		{
		case NONE:	
			{

			}
			break;
		case CONNECTION_REQUEST:
			{
				//printf("Someone found is trying to connect to you?\n");
			}
			break;
		case CONNECTION_APPROVED:
			{
				//printf("Someone is trying to Approve you? \n");
			}
			break;
		case CONNECTION_DENIED:
			{
				//printf("Someone is trying to Deny you? \n");
			}
			break;
		case CONNECTION_DENIED_USERNAME:
			{
				//printf("Someone is trying to Deny your name? \n");
			}
			break;
		case CONNECTION_UNAVALIABLE:
			{
				//printf("Someone is trying to tell you they are unavaliable\n");
			}
			break;
		case CONNECTION_END:
			{
				RemoveClient(m_RecievedPacketData);
			}
			break;
		case SERVER_ALERT:
			{
				//printf("Server Alert?\n");
			}
			break;
		case SERVER_KEEP_ALIVE:
			{
				//Taken Care of Above
				//printf("%\n");
			}
			break;
		case CLIENT_ALIVE_RESPONCE:
			{
				//printf("< %s : %s > - ALIVE \n", m_RecievedPacketData.t_Address.c_str(), m_RecievedPacketData.t_Port.c_str());
			}
			break;
		case SEND_MESSAGE:
			{
				//if(m_RecievedPacketData.t_Message != "")
				//{
					//RelayMessage();
				//}
			}
			break;
		case BROADCAST_REQUEST:
			{
				if(!m_GameRunning)
				{
					TUser ResponcePacket(m_OperatorData);
					ResponcePacket.t_MessageType = BROADCAST_REPLY;
					SendMessage(ResponcePacket.ToPacket(), m_RecievedPacketData.t_SocAddress);
				}
			}
			break;
		case BROADCAST_REPLY:
			{
				//printf("BROADCAST reply??\n");
			}
			break;
		case DISPLAY_USERS:
			{
			/* CHANGED */
				TUser ResponcePacket(m_OperatorData);
				ResponcePacket.t_MessageType = DISPLAY_USERS;
				ResponcePacket.t_Message = GetUsers();
				SendMessage(ResponcePacket.ToPacket(), m_RecievedPacketData.t_SocAddress);	
			}
			break;
		case DISPLAY_COMMANDS:
			{
				TUser ResponcePacket(m_OperatorData);
				ResponcePacket.t_Message = "<SERVER: Commands(!? - Help, !q - Disconnect, !h - Display users)>\n";
				ResponcePacket.t_MessageType = SERVER_ALERT;
				SendMessage(ResponcePacket.ToPacket(), m_RecievedPacketData.t_SocAddress);	
			}
			break;
		case CLIENT_KEEP_ALIVE:
			{
				TUser ResponcePacket(m_OperatorData);
				ResponcePacket.t_Message = "";
				ResponcePacket.t_MessageType = SERVER_ALIVE_RESPONCE;
				SendMessage(ResponcePacket.ToPacket(), m_RecievedPacketData.t_SocAddress);
			}
			break;
		}
	}
	else
	{
		switch((int)m_RecievedPacketData.t_MessageType)
		{
		case NONE:	
			{

			}
			break;
		case CONNECTION_REQUEST:
			{
				ConnectionRequest();
			}
			break;
		case CONNECTION_APPROVED:
			{
				//printf("Someone unknown is trying to Approve you? \n");
			}
			break;
		case CONNECTION_DENIED:
			{
				printf("Someone unknown is trying to Deny you? \n");
			}
			break;
		case CONNECTION_DENIED_USERNAME:
			{
				//printf("Someone unknown is trying to Deny your name? \n");
			}
			break;
		case CONNECTION_UNAVALIABLE:
			{
				//printf("Someone unknown is trying to tell you they are unavaliable\n");
			}
			break;
		case CONNECTION_END:
			{
				//printf("Someone unknown is trying to tell you they are unavaliable\n");
			}
			break;
		case SERVER_ALERT:
			{
				//printf("Unknown Server Alert?\n");
			}
			break;
		case SERVER_KEEP_ALIVE:
			{
				//printf("Unknown Server keep alive?\n");
			}
			break;
		case CLIENT_ALIVE_RESPONCE:
			{
				//printf("Unknown Client Alive Message?\n");
			}
			break;
		case SEND_MESSAGE:
			{
				//printf("Unknown Message?\n");
			}
			break;
		case BROADCAST_REQUEST:
			{
				//printf("< %s : %s > - Broadcast\n", m_RecievedPacketData.t_Address.c_str(), m_RecievedPacketData.t_Port.c_str());
				if(!m_GameRunning)
				{
					TUser ResponcePacket(m_OperatorData);
					ResponcePacket.t_MessageType = BROADCAST_REPLY;
					SendMessage(ResponcePacket.ToPacket(), m_RecievedPacketData.t_SocAddress);
				}
			}
			break;
		case BROADCAST_REPLY:
			{
				//printf("Unknown BROADCAST reply??\n");
			}
			break;
		case DISPLAY_USERS:
			{
				//printf("Unknown BROADCAST reply??\n");
			}
			break;
		}
	}
}

void Operator::ClientHandler()
{
	switch((int)m_RecievedPacketData.t_MessageType)
	{
	case NONE:	
		{

		}
		break;
	case CONNECTION_REQUEST:
		{
			//printf("Someone is trying to connect to you?\n");
		}
		break;
	case CONNECTION_APPROVED:
		{
			//printf(m_RecievedPacketData.t_Message.c_str());
			m_ClientConnected = true;
		}
		break;
	case CONNECTION_DENIED:
		{
			//printf(m_RecievedPacketData.t_Message.c_str());
		}
		break;
	case CONNECTION_DENIED_USERNAME:
		{
			//printf("%s\n", m_RecievedPacketData.t_Message.c_str());
		}
		break;
	case CONNECTION_UNAVALIABLE:
		{
			//printf(m_RecievedPacketData.t_Message.c_str());
		}
		break;
	case CONNECTION_END:
		{
			//printf("Server Connection Ended.\n");
			m_ClientConnected = false;
		}
		break;
	case SERVER_ALERT:
		{
			printf(m_RecievedPacketData.t_Message.c_str());
		}
		break;
	case SERVER_KEEP_ALIVE:
		{
			TUser ResponcePacket(m_OperatorData);
			ResponcePacket.t_MessageType = CLIENT_ALIVE_RESPONCE;
			SendMessage(ResponcePacket.ToPacket(), m_sServerAddress);
		}
		break;
	case CLIENT_ALIVE_RESPONCE:
		{
			//printf("Client Alive Message?\n");
		}
		break;
	case SEND_MESSAGE:
		{
			//printf("%s : %s \n", m_RecievedPacketData.t_Username.c_str(), m_RecievedPacketData.t_Message.c_str());
		}
		break;
	case BROADCAST_REQUEST:
		{
			//printf("Someone is broadcasting\n");
		}
		break;
	case BROADCAST_REPLY:
		{
			m_vClientSocketAddresses.push_back(m_RecievedPacketData);
		}
		break;
	case DISPLAY_USERS:
		{
			//printf("Someone wants your users?\n");
		}
		break;
	case SERVER_ALIVE_RESPONCE:
		{
			m_CurrentTime = 0;
			m_bServerCheck = false;
		}
	}
}

void Operator::ConnectionRequest()
{
	bool bSameName = false;

	if(m_vClientSocketAddresses.size() > 1)
	{
		bSameName = true;

		while(bSameName)
		{
			bSameName = false;

			for(unsigned int i = 0; i < m_vClientSocketAddresses.size(); i++)
			{
				if(strcmp(m_RecievedPacketData.t_Username.c_str(), m_vClientSocketAddresses[i].t_Username.c_str()) == 0)
				{
					bSameName = true;
				}
			}

			if(bSameName) {m_RecievedPacketData.t_Username += "1";}
		}
	}

	if(!bSameName)
	{
		//printf("< %s : %s > - Connected\n", m_RecievedPacketData.t_Address.c_str(), m_RecievedPacketData.t_Port.c_str());

		for(unsigned int i = 0; i < m_vClientSocketAddresses.size(); i++)
		{
			//ALERT OTHER CHATTERS 
			TUser AlertPacket(m_OperatorData);
			AlertPacket.t_Message = "<SERVER: ";
			AlertPacket.t_Message += m_RecievedPacketData.t_Username;
			AlertPacket.t_Message += " joined the Chat!>\n";
			AlertPacket.t_MessageType = SERVER_ALERT;
			SendMessage(AlertPacket.ToPacket(), m_vClientSocketAddresses[i].t_SocAddress);
		}

		std::string newMessage = "<SERVER: Connected!>\n<";
		newMessage += GetUsers();
		newMessage += ">\n";
		m_vClientSocketAddresses.push_back(m_RecievedPacketData);

		TUser NewPacket(m_OperatorData);
		NewPacket.t_Message = newMessage;
		NewPacket.t_MessageType = CONNECTION_APPROVED;
		SendMessage(NewPacket.ToPacket(), m_RecievedPacketData.t_SocAddress);
	}
	else
	{
		// Fail if username taken
		TUser FailPacket(m_OperatorData);
		FailPacket.t_Message = "<SERVER: Connection Denied : Choose a new UserName>\n";
		FailPacket.t_MessageType = CONNECTION_DENIED_USERNAME;
		SendMessage(FailPacket.ToPacket(), m_RecievedPacketData.t_SocAddress);
	}
}



void Operator::RemoveClient(TUser _user)
{
	for(unsigned int i = 0; i < m_vClientSocketAddresses.size(); i++)
	{
		if((strcmp(m_vClientSocketAddresses[i].t_Address.c_str(), _user.t_Address.c_str()) == 0) && 
			(strcmp(m_vClientSocketAddresses[i].t_Port.c_str(), _user.t_Port.c_str()) == 0))
		{
			TUser EndConnection(m_OperatorData);
			EndConnection.t_Message = "Connection to Server Terminated";
			EndConnection.t_MessageType = CONNECTION_END;
			SendMessage(EndConnection.ToPacket(), _user.t_SocAddress);

			//printf("< %s : %s > - Disconnected\n", m_vClientSocketAddresses[i].t_Address.c_str(), m_vClientSocketAddresses[i].t_Port.c_str());
			m_vClientSocketAddresses.erase(m_vClientSocketAddresses.begin() + i);
		}
		else
		{
			//ALERT OTHER CHATTERS 
			TUser AlertPacket(m_OperatorData);
			AlertPacket.t_Message = "<";
			AlertPacket.t_Message += _user.t_Username;
			AlertPacket.t_Message += " disconnected>\n";
			AlertPacket.t_MessageType = SERVER_ALERT;
			SendMessage(AlertPacket.ToPacket(), m_vClientSocketAddresses[i].t_SocAddress);
		}
	}
}

void Operator::RecieveMessage()
{
	m_iReady--;

	m_iSizeofAddress = sizeof( m_sRetrivedAddress );
	m_iNumberofBites = recvfrom(							// pulls a packet from a single source...
		m_Socket,											// server socket to read from
		m_PacketData,										// incoming packet to be filled
		uMAX_MESSAGE_LENGTH,								// lengh of incoming packet to be filled
		0,													// flags
		reinterpret_cast<sockaddr*>(&m_sRetrivedAddress),	// address to be filled with packet source
		&m_iSizeofAddress									// size of the above address struct.
		);

	if(m_iNumberofBites < 0)
	{
		m_PacketData[0] = 0;
		m_RecievedPacketData.Clear();
	}
	else
	{
		m_RecievedPacketData.FromPacket(m_PacketData);
		m_RecievedPacketData.t_SocAddress = m_sRetrivedAddress;
	}
}

void Operator::SendMessage(std::string _packet, sockaddr_in _Address)
{
	/*Sends packet to passed address */
	int iNumToSend = (int)strlen( _packet.c_str() ) + 1;

	m_iNumberofBites = sendto(
		m_Socket,													// socket to send through.
		_packet.c_str(),											// data to send
		iNumToSend,													// num bytes to send
		0,															// flags
		reinterpret_cast<sockaddr*>(&_Address),						// address to be filled with packet target
		sizeof(_Address	)											// size of the above address struct.
		);
	m_iNumberofBites;
}

void Operator::RelayMessage(EMessageType _MType, std::string _Message)
{
	TUser ToSendPacket(m_OperatorData);
	ToSendPacket.t_Message = _Message;
	ToSendPacket.t_MessageType = _MType;
	
	/* Pass it to all Connected */
	for(unsigned int i = 0; i < m_vClientSocketAddresses.size(); i++)
	{
		SendMessage(ToSendPacket.ToPacket(), m_vClientSocketAddresses[i].t_SocAddress);
	}
}

void Operator::SendClientMessage(EMessageType _MType, std::string _Message)
{
	TUser ToSendPacket(m_OperatorData);
	ToSendPacket.t_Message = _Message;
	ToSendPacket.t_MessageType = SEND_MESSAGE;
	SendMessage(ToSendPacket.ToPacket(), m_sServerAddress);
}

std::string Operator::GetUsers()
{
	std::string newMessage = "";
	for(unsigned int i = 0; i < m_vClientSocketAddresses.size(); i++)
	{
		newMessage += m_vClientSocketAddresses[i].t_Username;
		if(i != m_vClientSocketAddresses.size() - 1)
		{
			newMessage += ",";
		}
	}
	newMessage += ",";
	return newMessage;
}

void Operator::CheckClients()
{
	for(unsigned int i = 0; i < m_vClientSocketAddresses.size(); i++)
	{
		m_vClientSocketAddresses[i].t_AliveTime += m_TTimer.getDeltaTime();
		if((m_vClientSocketAddresses[i].t_AliveTime > m_MaxTime))
		{
			if(m_vClientSocketAddresses[i].t_bCheck)
			{
				RemoveClient(m_vClientSocketAddresses[i]);
			}
			else
			{
				m_vClientSocketAddresses[i].t_bCheck = true;
				m_vClientSocketAddresses[i].t_AliveTime = 0;
				TUser AlertPacket(m_OperatorData);
				AlertPacket.t_Message = "<SERVER: ALIVE_CHECK>";
				AlertPacket.t_MessageType = SERVER_KEEP_ALIVE;
				SendMessage(AlertPacket.ToPacket(), m_vClientSocketAddresses[i].t_SocAddress);
			}
		}
	}
}

void Operator::CheckServer()
{
	m_CurrentTime += m_TTimer.getDeltaTime();
	if(m_CurrentTime > m_MaxTime)
	{
		m_CurrentTime = 0;
		if(m_bServerCheck)
		{
			// SERVER DROP
		}
		else
		{
			m_bServerCheck = true;
			TUser AlertPacket(m_OperatorData);
			AlertPacket.t_Message = "<SERVER: ALIVE_CHECK>";
			AlertPacket.t_MessageType = CLIENT_KEEP_ALIVE;
			SendMessage(AlertPacket.ToPacket(), m_sServerAddress);
		}
	}
}

TUser& Operator::Run()
{
	FD_SET ReadDesc;
	FD_SET ErrorDesc;
	m_RecievedPacketData.Clear();
	m_tv.tv_sec = 0;

	m_TTimer.tick();

	/* Check Alive */
	if (!m_bServer)
	{
		CheckServer();
	}
	else
	{
		CheckClients();
	}

	/* Set Variables */
	FD_ZERO(&ErrorDesc);
	FD_SET(m_Socket, &ErrorDesc);
	FD_ZERO(&ReadDesc);
	FD_SET(m_Socket, &ReadDesc);

	/* Wait for input */
	m_iReady = select(1, &ReadDesc, NULL, &ErrorDesc, m_pTimeVal);

	/* Operator Recieve Message */
	if (FD_ISSET(m_Socket, &ReadDesc))
	{
		RecieveMessage();
		HandlePacket();
	}

	return m_RecievedPacketData;
}

bool Operator::IsServer()
{
	return m_bServer;
}

std::vector<TUser>& Operator::RetrieveUsers()
{
	return m_vClientSocketAddresses;
}