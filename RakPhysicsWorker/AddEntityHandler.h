#pragma once
#include "stdafx.h"
#include "ServVars.h"
#include "Shapes.h"
#include "EntityMotionState.h"
#include "btBulletDynamicsCommon.h"

/*																				____________________
																				   \__________/    |
"Add entity" packet structure:														|  C++    |    |
- World id: id of a world that entity mst be spawned in(Integer)                /------------------|\
- Entity id: entity unique id(Integer)                                         /                   |\\
- X: x pos(Float)                                                             /   /===+      /===+ O \\
- Y: y pos(Float)                                                             |  /00  |     /00  |   ||
- Z: z pos(Float)                                                             |  \0---/     \0---/   ||
- Shape definition: what type of physics shape must be used(Short)            |                      ||
- Has mass: true if we must read mass, false if dont(Bool)                    |   \---------------|  ||
- Mass(Opt): mass of an entity(Float)                                         |    \=========     /  ||
                                                                               \     \-----------/   /
if shape definition is BOX:													    \___________________/
- Height: height of a box(Float)											    

if shape definition is CONE, CAPSULE or CYLLINDER:
- Height: height of a figure(Float)
- Radius: radius of a figure(Float)

if shape definition is SPHERE
- Radius: radius of a sphere(Float)

*/
void handleEntity(RakNet::Packet* p)
{

	RakNet::BitStream bsIn(p->data, p->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

	int worldId, entId;
	float x, y, z, mass = 50.f;
	bool hasMass;
	Shapes shapeEn;

	bsIn.Read(worldId);
	bsIn.Read(entId);
	bsIn.Read(x);
	bsIn.Read(y);
	bsIn.Read(z);
	bsIn.Read(shapeEn);
	bsIn.Read(hasMass);

	if (hasMass)
		bsIn.Read(mass);

	btCollisionShape* shape;
	float height, radius;
	float scale;

	switch (shapeEn)
	{
	case BOX:
		bsIn.Read(scale);
		shape = new btBoxShape(*new btVector3(scale / 2, scale / 2, scale / 2));
		break;
	case CYLLINDER:
		bsIn.Read(height);
		bsIn.Read(radius);
		shape = new btCylinderShape(*new btVector3(radius, height, radius));
		break;
	case SPHERE:
		bsIn.Read(radius);
		shape = new btSphereShape(radius);
		break;
	case CAPSULE:
		bsIn.Read(height);
		bsIn.Read(radius);
		shape = new btCapsuleShape(radius, height);
		break;
	case CONE:
		bsIn.Read(height);
		bsIn.Read(radius);
		shape = new btConeShape(radius, height);
		break;
	default:
		break;
	}

	World* w = ((worlds.find(worldId))->second);
	if (w != nullptr)
	{
		Entity e = *new Entity(entId, new btRigidBody(mass, new EntityMotionState(&e), shape), worldId);
		w->addEntity(e.setPosition(x, y, z));
	}
};