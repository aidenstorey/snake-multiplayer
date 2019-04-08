#ifndef GUARD_USER
#define GUARD_USER

#include <WinSock2.h>
#include <string>

enum EMessageType
{
	NONE,
	CONNECTION_REQUEST,
	CONNECTION_APPROVED,
	CONNECTION_DENIED,
	CONNECTION_DENIED_USERNAME,
	CONNECTION_UNAVALIABLE,
	CONNECTION_END,
	SERVER_ALERT,
	SERVER_KEEP_ALIVE,
	CLIENT_ALIVE_RESPONCE,
	CLIENT_KEEP_ALIVE,
	SERVER_ALIVE_RESPONCE,
	SEND_MESSAGE,
	BROADCAST_REQUEST,
	BROADCAST_REPLY,
	DISPLAY_USERS,
	DISPLAY_COMMANDS,
	STATE_MESSAGE,
	SCORE_MESSAGE,
	READY_MESSAGE,
};

struct TUser
{
	std::string		t_Address;
	std::string		t_Port;
	std::string		t_Username;
	std::string		t_Message;

	sockaddr_in		t_SocAddress;
	EMessageType	t_MessageType;

	float			t_AliveTime;
	bool			t_bCheck;
	bool			t_Ready;

	TUser()
	{
		Clear();
		t_AliveTime = 0;
		t_bCheck = false;
		t_Ready = false;
	}

	void Clear()
	{
		t_Address		= "";
		t_Port			= "";
		t_Username		= "";
		t_Message		= "";
		t_MessageType = NONE;
	}

	std::string ToPacket()
	{
		std::string NewPacket;

		/* For Messagetype */
		long double dTemp = (long double)t_MessageType;
		std::string f = std::to_string(dTemp);

		NewPacket += f;
		/* The Remainder*/
		NewPacket += ';';
		NewPacket += t_Address;
		NewPacket += ';';
		NewPacket += t_Port;
		NewPacket += ';';
		NewPacket += t_Username;
		NewPacket += ';';
		NewPacket += t_Message;
		NewPacket += ';';
		return NewPacket;
	}

	void FromPacket(std::string _Packet)
	{
		Clear();
		int iNum = 0;
		int iIndex = 0;

		while(iNum != 5)
		{
			if(_Packet.c_str()[iIndex] == ';')
			{
				iNum++;
				iIndex++;
			}
			else
			{
				switch(iNum)
				{
				case 0:
					t_MessageType = (EMessageType)(atoi(_Packet.c_str()));
					break;
				case 1:
					t_Address += _Packet.c_str()[iIndex];
					break;
				case 2:
					t_Port += _Packet.c_str()[iIndex];
					break;
				case 3:
					t_Username += _Packet.c_str()[iIndex];
					break;
				case 4:
					t_Message += _Packet.c_str()[iIndex];
					break;
				}
				iIndex++;
			}
		}
	}

	void ReadCommand()
	{
		std::string sTemp;
		unsigned int i = 0;
		if(t_Message.size() > 0)
		{
			if(t_Message[0] == '!')
			{
				while(t_Message.size() > i)
				{
					if(t_Message[i] == '!')
					{

					}
					else
					{
						sTemp += t_Message[i];
					}
					i++;
				}

				if(strcmp(sTemp.c_str(), "?") == 0)
				{
					t_MessageType = DISPLAY_COMMANDS;
				}
				else if(strcmp(sTemp.c_str(), "q") == 0)
				{
					t_MessageType = CONNECTION_END;
				}
				else if(strcmp(sTemp.c_str(), "b") == 0)
				{
					t_MessageType = BROADCAST_REQUEST;
				}
				else if(strcmp(sTemp.c_str(), "h") == 0)
				{
					t_MessageType = DISPLAY_USERS;
				}
				else if(strcmp(sTemp.c_str(), "c") == 0)
				{
					t_MessageType = CONNECTION_REQUEST;
				}
			}
			else
			{
				t_MessageType = SEND_MESSAGE;
			}
		}
	}
};
#endif

