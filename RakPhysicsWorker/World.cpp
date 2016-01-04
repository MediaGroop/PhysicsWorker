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
#if defined(_WIN64) || defined(_WIN32)
	this->_trd = new std::thread(this->step, this, 100, grav);
#else
      	pthread_t* trd = new pthread_t();
	world_data data2;
	data2._inst = this;
	data2._freq = 100;
	data2._x = grav.x();
	data2._y = grav.y();
	data2._z = grav.z();
	pthread_create(trd, NULL, &this->step, (void *)&data2);
	this->_trd = trd;
#endif
}

#if defined(_WIN64) || defined(_WIN32)
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
#else
void* World::step(void * data)
{
	world_data* w_data = (struct world_data *)data;
	World* w = w_data->_inst;
	btVector3 grav = *new btVector3(w_data->_x, w_data->_y, w_data->_z);
	
	w->_collisionConfiguration = new btDefaultCollisionConfiguration();
	w->_dispatcher = new btCollisionDispatcher(w->_collisionConfiguration);
	w->_overlappingPairCache = new btDbvtBroadphase();
	w->_solver = new btSequentialImpulseConstraintSolver;
	w->_dynamicsWorld = new btDiscreteDynamicsWorld(w->_dispatcher, w->_overlappingPairCache, w->_solver, w->_collisionConfiguration);
	w->_dynamicsWorld->setGravity(grav);
	
	struct timespec req;
	req.tv_sec = 0;
	req.tv_nsec = w_data->_freq * 1000;
	
	while (w->getRunning()){
	//	LOG(INFO) << "step";
		nanosleep(&req, NULL);
		w->_dynamicsWorld->stepSimulation(1.f / 60.f, 10.0f);
	}
}
#endif
World::~World()
{
	delete _dynamicsWorld;
	delete _solver;
	delete _overlappingPairCache;
	delete _dispatcher;
	delete _collisionConfiguration;
}
