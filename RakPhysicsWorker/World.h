#pragma once
#include <map>
#include "Entity.h"
#include "btBulletDynamicsCommon.h"
#if defined(_WIN64) || defined(_WIN32) 
#include <thread>
#else
#include <pthread.h>
#endif

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
	
#if defined(_WIN64) || defined(_WIN32)
	std::thread* _trd;
#else
	pthread_t* _trd;
#endif
	
	bool _running;
public:
	void removeEntity(int);
	void addEntity(Entity*);
	void addStatic(); // WIP
	World(int, btVector3&);
#if defined(_WIN64) || defined(_WIN32)
	static void step(World*, long, btVector3&);
#else
	static void* step(void* data);
#endif
	~World();

	bool getRunning(){
		return _running;
	};

	Entity* getEntity(int id)
	{
		return &_entities.find(id)->second;
	}
};

#if !defined(_WIN64) && !defined(_WIN32)
struct world_data{
    World* _inst;
    long   _freq;
    float  _x;
    float  _y;
    float  _z;
};
#endif
