#pragma once
#include "btBulletDynamicsCommon.h"
#include "Entity.h"

class EntityMotionState :
	public btMotionState
{
private:
	Entity* _entity;
public:
	EntityMotionState(Entity*);
	~EntityMotionState();

	virtual void getWorldTransform(btTransform& worldTrans) const override;

	virtual void setWorldTransform(const btTransform& worldTrans) override;


};

