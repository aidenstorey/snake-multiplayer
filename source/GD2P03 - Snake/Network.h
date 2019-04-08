#ifndef GUARD_NETWORK_H
#define GUARD_NETWORK_H

// Library Includes
#include <string>

// Local Includes
#include "Operator.h"

// Enumerations
enum ECOMMAND
{
	EMPTY,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SCORES,
	STATE,
	QUIT,
	READY,
	USERS,
};

// Prototype
class Network
{
public:
	Network();
	~Network();

	bool Setup(std::string _sName, bool _bServer);
	bool ClientConnect(std::string _ip);
	std::vector<std::string> Broadcast();
	std::string GetTextMessage();
	void Send(ECOMMAND _Message, std::string sState);
	ECOMMAND Recieve();
	bool CheckClientsReady();
	bool IsGameReady();
	void ReadyClient(TUser& _Packet);

	std::string GetUserNames();

	std::vector<TUser>& Network::GetClients();
	std::vector< std::string > UserNamesClient();

	void Reset();

	bool IsServer();

	bool IsGameRunning();

	/* For Tesing Only */
	std::string GetStr();

	int GetRecentClient();

	/* Private */
	void Run();

private:
	Operator*				m_Operator;
	std::vector<TUser>		m_Servers;
	std::vector<TUser>*		m_Clients;
	TUser					m_CurrentPacket;
};

#endif // GUARD_NETWORK_H