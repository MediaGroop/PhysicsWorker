#pragma once
#include "stdafx.h"
#include "ServVars.h"

void handleRemoveEntity(RakNet::Packet* p)
{

	RakNet::BitStream bsIn(p->data, p->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

	int worldId, id;

	bsIn.Read(id);
	bsIn.Read(worldId);

	World* w = ((*worlds.find(worldId)).second);
	if (w != nullptr)
	{
		w->removeEntity(id);
	}
};