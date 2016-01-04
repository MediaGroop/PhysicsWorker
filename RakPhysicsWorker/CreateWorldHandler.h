#pragma once
#include "stdafx.h"
#include "ServVars.h"

void handleCreateWorld(RakNet::Packet* p)
{

	RakNet::BitStream bsIn(p->data, p->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	
	int id;
	float x, y, z;

	bsIn.Read(id);
	bsIn.Read(x);
	bsIn.Read(y);
	bsIn.Read(z);

	worlds.insert(pair<int, World*>(id, new World(id, *new btVector3(x, y, z))));
};