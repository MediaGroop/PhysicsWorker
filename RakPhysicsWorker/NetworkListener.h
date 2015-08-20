#pragma once
#include "RakPeerInterface.h"
#include <map>
#include <functional>

class NetworkListener
{
private:
	std::map<short, std::function<void(RakNet::Packet*)>> _handlers;
public:
	NetworkListener(){};
	~NetworkListener(){};
	void handle(RakNet::Packet* packet);
	void add(short, std::function<void(RakNet::Packet*)>);
};

