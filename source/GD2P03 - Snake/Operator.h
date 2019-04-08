#ifndef _OPERATOR__H__
#define _OPERATOR__H__

// Library Includes
#include <assert.h>
#include <string>
#include <vector>
#include <WinSock2.h>

//#include "Includes.h"
#include "User.h"
#include "GameTimer.h"

// Constant Variables
const static int iMAX_CLIENTS = 10;
const static int iNET_SLEEP = 30;

const static unsigned short uDEFAULT_SERVER_PORT = (unsigned short)60012;
const static unsigned short uDEFAULT_CLIENT_PORT = (unsigned short)60013;
const static unsigned short uDEFAULT_BROADCAST_PORT = (unsigned short)60014;

const static UINT32 uMAX_MESSAGE_LENGTH = 1000;

// Prototyps
class Operator
{
public:
	Operator();
	~Operator();

	bool SetServer(std::string _name);
	bool SetClient(std::string _name);

	bool ClientConnect(std::string _ip);
	std::vector<TUser>& ClientBroadcast();

	void SetPorts();
	void SetCurrentAddress();
	void BindSocket();
	void BroadcastSocket();

	void SendClientInput();
	void HandlePacket();

	void SendMessage(std::string _packet, sockaddr_in _Address);
	void RelayMessage(EMessageType _MType, std::string _Message);
	void SendClientMessage(EMessageType _MType, std::string _Message);
	void RecieveMessage();

	TUser& Run();

	std::vector<TUser>& RetrieveUsers();

	std::string GetUsers();
	void RemoveClient(TUser _user);
	void ConnectionRequest();

	void ServerHandler();
	void ClientHandler();

	void CheckClients();
	void CheckServer();

	void HandleBroadcast();
	void WaitForMessages();

	bool IsServer();

	bool IsRunning();
	void SetRunningState(bool _t);

private:
	bool						m_bServer;

	/* Server IP */
	char						m_cServerIP[128];

	std::vector<TUser>			m_vClientSocketAddresses;

	unsigned short				m_uCLIENT_PORT;
	unsigned short				m_uSERVER_PORT;
	unsigned short				m_uBROADCAST_PORT;

	/* Server Address for Client   */
	sockaddr_in					m_sServerAddress;

	/* Address of Current Operator */
	sockaddr_in					m_sCurrentAddress;

	/* Recieved From */
	sockaddr_in					m_sRetrivedAddress;

	/* Buffer for packet Data      */
	char						m_PacketData[uMAX_MESSAGE_LENGTH];

	/* For Socket */
	SOCKET						m_Socket;

	/* Time for instnace*/
	TIMEVAL*					m_pTimeVal;
	TIMEVAL						m_tv;

	/*Varibales for packet*/
	int							m_iReady;
	int							m_iSizeofAddress;
	int							m_iNumberofBites;
	int							m_iMessageNum;

	/* Packet Data for Sending data */
	TUser						m_OperatorData;

	/* Packet Data for Recieveing data */
	TUser						m_RecievedPacketData;

	bool						m_ClientConnected;

	GameTimer					m_TTimer;
	float						m_CurrentTime;
	float						m_MaxTime;

	bool						m_bServerCheck;

	std::string					m_sSeverName;

	bool						m_GameRunning;
};

#endif