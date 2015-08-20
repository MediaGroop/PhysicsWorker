#pragma once
#include "BulletDynamics\Dynamics\btRigidBody.h"

class Entity
{
private:
	btTransform _transform;
	btRigidBody* _body;
	int _id;
	int _world_id;
public:
	Entity(int, btRigidBody*, int);
	~Entity();
	Entity* setPosition(float, float, float);
	Entity* setPosition(btVector3&);

	int getWorldId()
	{
		return _world_id;
	}

	btTransform* getTransform()
	{
		return &_transform;
	}

	int getId()
	{
		return _id;
	}
	btRigidBody* getBody()
	{
		return _body;
	}
};

