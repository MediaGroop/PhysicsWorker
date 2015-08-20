#pragma once
#include "stdafx.h"
#include <thread>
#include "ConnectedClient.h"
#include "NetworkListener.h"

using namespace std;

class Server
{
private:

	RakNet::RakPeerInterface *peer;
	NetworkListener* listener;

	std::thread *networkTrd;

	bool running = false;
	bool secure = false;

	map<RakNet::RakNetGUID, ConnectedClient> _connections;
public:	

	void setThread(std::thread* trd)
	{
		this->networkTrd = trd;
	}

	void setRunning(bool r)
	{
		running = r;
	}

	void setSecure(bool s)
	{
		secure = s;
	}

	std::thread* getThread(){
		return networkTrd;
	}

	bool isSecure(){
		return secure;
	}

	bool isRunning(){
		return running;
	}

	NetworkListener* getListener()
	{
		return this->listener;
	}

	RakNet::RakPeerInterface* getPeer()
	{
		return this->peer;
	}

	map<RakNet::RakNetGUID, ConnectedClient>* get_connections(){
		return &_connections;
	}

	Server(NetworkListener * lis){
		this->listener = lis;
		this->peer = RakNet::RakPeerInterface::GetInstance();

	};
	static void startNetworkTrd(Server*, int, int);

	~Server(){};
	
	//Another ghost function
	bool initSecurity(const char*, const char*);

	void addClient(RakNet::RakNetGUID, ConnectedClient);
	bool hasClient(RakNet::RakNetGUID);
	ConnectedClient* getClient(RakNet::RakNetGUID);
	void removeClient(RakNet::RakNetGUID);
};