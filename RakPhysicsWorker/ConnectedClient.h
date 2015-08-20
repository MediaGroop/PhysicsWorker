#pragma once
#include "RakPeerInterface.h"
#include "easylogging++.h"

class ConnectedClient
{
private:
	RakNet::AddressOrGUID addr;
public:

	RakNet::AddressOrGUID* getAddr()
	{
		return &addr;
	}

	ConnectedClient(RakNet::AddressOrGUID a){
		this->addr = a;
	};

	virtual ~ConnectedClient(){
		LOG(INFO) << "ConnectedClient dctor";
	};
};