#pragma once
#include <map>
#include "Entity.h"
#include "BulletDynamics\ConstraintSolver\btSequentialImpulseConstraintSolver.h"
#include "BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h"
#include "BulletCollision\CollisionDispatch\btDefaultCollisionConfiguration.h"
#include <thread>

class World
{
private:
	std::map<int, Entity> _entities; 
	
	int _id;
	
	btDefaultCollisionConfiguration * _collisionConfiguration;
	btCollisionDispatcher * _dispatcher;
	btBroadphaseInterface * _overlappingPairCache;
	btSequentialImpulseConstraintSolver * _solver;
	btDiscreteDynamicsWorld * _dynamicsWorld;
	
	std::auto_ptr<std::thread> _trd;
	
public:
	void removeEntity(int);
	void addEntity(Entity*);
	void addStatic(); // WIP
	World(int, btVector3&);
	void step(long);
	~World();
};

