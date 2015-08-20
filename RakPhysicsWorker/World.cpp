#include "World.h"
#include "BulletCollision\BroadphaseCollision\btDbvtBroadphase.h"

void startSimulation(World* w, long d)
{
	w->step(d);
}

void World::removeEntity(int id)
{
	Entity* e = &((*_entities.find(id)).second);
	_entities.erase(id);
	_dynamicsWorld->removeRigidBody(e->getBody());
	delete e;
};

void World::addEntity(Entity* e)
{
	_entities.insert(std::pair<int, Entity>(e->getId(), *e));
	_dynamicsWorld->addRigidBody(e->getBody());
};

World::World(int i, btVector3& grav)
{
	this->_collisionConfiguration = new
		btDefaultCollisionConfiguration();
	this->_dispatcher = new btCollisionDispatcher(_collisionConfiguration);
	this->_overlappingPairCache = new btDbvtBroadphase();
	this->_solver = new btSequentialImpulseConstraintSolver;
	this->_dynamicsWorld = new btDiscreteDynamicsWorld(_dispatcher,
		_overlappingPairCache, _solver, _collisionConfiguration);
	this->_dynamicsWorld->setGravity(grav);
	this->_trd = std::auto_ptr<std::thread>(&std::thread(startSimulation, this, 100));
	this->_id = i;
}

void World::step(long delay)
{
	this->_dynamicsWorld->stepSimulation(1.f / 60.f, 10.0f);
}

World::~World()
{
	delete _dynamicsWorld;
	delete _solver;
	delete _overlappingPairCache;
	delete _dispatcher;
	delete _collisionConfiguration;
}
