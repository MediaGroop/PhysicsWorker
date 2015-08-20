#pragma once
#include "stdafx.h"
#include "ServVars.h"

void handleConnect(RakNet::Packet* packet)
{
	if (!mainServer->hasClient(packet->guid))
	{
		mainServer->addClient(packet->guid, *new ConnectedClient(packet->systemAddress));
	}
};