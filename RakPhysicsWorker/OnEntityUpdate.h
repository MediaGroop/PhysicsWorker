#pragma once
#include "stdafx.h"
#include "ServVars.h"

void onUpdateEntity(RakNet::Packet* p)
{

	RakNet::BitStream bsIn(p->data, p->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

	int worldId, id;
	float x, y, z;
	bsIn.Read(id);
	bsIn.Read(worldId);
	bsIn.Read(x);
	bsIn.Read(y);
	bsIn.Read(z);

	World* w = ((*worlds.find(worldId)).second);
	if (w != nullptr)
	{
		//TODO: update entity
		Entity* ent = w->getEntity(id);
		if (ent != nullptr)
		{
			ent->setPosition(x, y, z);
		}
	}
};