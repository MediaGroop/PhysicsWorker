#include "World.h"
#include "easylogging++.h"

#define sleep Sleep(10)

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
	this->_id = i;
	this->_running = true;
	this->_trd = new std::thread(this->step, this, 100, grav);
}

void World::step(World* w, long delay, btVector3& grav)
{

	w->_collisionConfiguration = new btDefaultCollisionConfiguration();
	w->_dispatcher = new btCollisionDispatcher(w->_collisionConfiguration);
	w->_overlappingPairCache = new btDbvtBroadphase();
	w->_solver = new btSequentialImpulseConstraintSolver;
	w->_dynamicsWorld = new btDiscreteDynamicsWorld(w->_dispatcher, w->_overlappingPairCache, w->_solver, w->_collisionConfiguration);
	w->_dynamicsWorld->setGravity(grav);
	while (w->getRunning()){
	//	LOG(INFO) << "step";
		sleep;
		w->_dynamicsWorld->stepSimulation(1.f / 60.f, 10.0f);
	}
}

World::~World()
{
	delete _dynamicsWorld;
	delete _solver;
	delete _overlappingPairCache;
	delete _dispatcher;
	delete _collisionConfiguration;
}
