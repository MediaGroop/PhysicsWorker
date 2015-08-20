#pragma once
#include "stdafx.h"
#include "ServVars.h"

void handleDisconnect(RakNet::Packet* p)
{
	if (mainServer->hasClient(p->guid))
	{
		mainServer->removeClient(p->guid);
	}
};