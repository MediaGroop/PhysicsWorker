#include "stdafx.h"
#include "EntityMotionState.h"
#include "PacketTypes.h"
#include "ServVars.h"

EntityMotionState::EntityMotionState(Entity* e) : _entity(e)
{
};


EntityMotionState::~EntityMotionState()
{
};

void EntityMotionState::setWorldTransform(const btTransform& worldTrans)
{
	_entity->getTransform()->operator=(worldTrans);
	static RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)UPDATE_ENTITY);
	bsOut.Write(_entity->getWorldId());

	bsOut.Write(_entity->getId());
	bsOut.Write(_entity->getTransform()->getOrigin().x());
	bsOut.Write(_entity->getTransform()->getOrigin().y());
	bsOut.Write(_entity->getTransform()->getOrigin().z());
	//TODO: send velocity
	mainServer->getPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, nullptr, true);
};

void EntityMotionState::getWorldTransform(btTransform& worldTrans) const
{
	worldTrans = *_entity->getTransform();
};