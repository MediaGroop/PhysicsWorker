#pragma once
#include <map>
#include "Entity.h"
#include "btBulletDynamicsCommon.h"
#include <thread>

class World
{
private:
	std::map<int, Entity> _entities; 
	
	int _id;
	
	btDefaultCollisionConfiguration* _collisionConfiguration;
	btCollisionDispatcher* _dispatcher;
	btBroadphaseInterface* _overlappingPairCache;
	btSequentialImpulseConstraintSolver* _solver;
	btDiscreteDynamicsWorld* _dynamicsWorld;
	
	std::thread* _trd;
	bool _running;
public:
	void removeEntity(int);
	void addEntity(Entity*);
	void addStatic(); // WIP
	World(int, btVector3&);
	static void step(World*, long, btVector3&);
	~World();

	bool getRunning(){
		return _running;
	};

	Entity* getEntity(int id)
	{
		return &_entities.find(id)->second;
	}
};

