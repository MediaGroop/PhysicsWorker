#include "Entity.h"


Entity::Entity(int id, btRigidBody* bod, int wId) : _id(id), _body(bod), _world_id(wId)
{
	_body->setWorldTransform(_transform);
};

Entity::~Entity()
{
	delete _body;
};


Entity* Entity::setPosition(float x, float y, float z){
	return this->setPosition(*new btVector3(x, y, z));
};

Entity* Entity::setPosition(btVector3& pos){
	this->_transform.setOrigin(pos);
	_body->setWorldTransform(_transform);
	return this;
};
