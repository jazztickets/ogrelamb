/*************************************************************************************
*	ogrelamb - http://ogrelamb.googlecode.com
*	Copyright (C) 2012  Alan Witkowski
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************************/
#include "stdafx.h"
#include "physics.h"
#include "object.h"

_Physics Physics;

// Initializes the physics system
void _Physics::Init() {

	// Set up physics modules
	CollisionConfiguration = new btDefaultCollisionConfiguration();
	//BroadPhase = new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000));
	BroadPhase = new btDbvtBroadphase();
	GhostPair = new btGhostPairCallback();
	BroadPhase->getOverlappingPairCache()->setInternalGhostPairCallback(GhostPair);
	Dispatcher = new btCollisionDispatcher(CollisionConfiguration);
	Solver = new btSequentialImpulseConstraintSolver();
	World = new btFixedWorld(Dispatcher, BroadPhase, Solver, CollisionConfiguration);
	World->setGravity(btVector3(0.0f, -9.81f, 0.0f));

	Enabled = false;
}

// Closes the physics system
void _Physics::Close() {
	delete World;
	delete Solver;
	delete Dispatcher;
	delete BroadPhase;
	delete CollisionConfiguration;
	delete GhostPair;

	Enabled = false;
}

// Updates the physics system
void _Physics::Update(float FrameTime) {

	if(!Enabled)
		return;

	// Update physics
	World->stepSimulation(FrameTime);

	// Handle collision callbacks
	int ManifoldCount = World->getDispatcher()->getNumManifolds();
	for(int i = 0; i < ManifoldCount; i++) {
		btPersistentManifold *ContactManifold = World->getDispatcher()->getManifoldByIndexInternal(i);
		if(ContactManifold->getNumContacts() > 0) {
			const btCollisionObject *CollisionObject0 = static_cast<const btCollisionObject *>(ContactManifold->getBody0());
			const btCollisionObject *CollisionObject1 = static_cast<const btCollisionObject *>(ContactManifold->getBody1());
				
			//ObjectClass *Object0 = static_cast<ObjectClass *>(CollisionObject0->getUserPointer());
			//ObjectClass *Object1 = static_cast<ObjectClass *>(CollisionObject1->getUserPointer());

			//Object0->HandleCollision(Object1, ContactManifold, 1);
			//Object1->HandleCollision(Object0, ContactManifold, -1);
		}
	}
}

// Resets the physics world
void _Physics::Reset() {
	BroadPhase->resetPool(Dispatcher);
	Solver->reset();
}

// Performs raycasting on the world and returns the point of collision
bool _Physics::RaycastWorld(const btVector3 &Start, btVector3 &End, btVector3 &Normal) {
	if(!Enabled)
		return false;

	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	RayCallback.m_collisionFilterMask = FILTER_CAMERA;

	// Perform raycast
	World->rayTest(Start, End, RayCallback);
	if(RayCallback.hasHit()) {
		End = RayCallback.m_hitPointWorld;
		Normal = RayCallback.m_hitNormalWorld;

		return true;
	}

	return false;
}

// Removes a bit field from a value
void _Physics::RemoveFilter(int &Value, int Filter) {

	Value &= (~Filter);
}

// Sets default, static, or kinematic on a rigid body
void _Physics::SetBodyType(int &Value, int Filter) {

	Value &= (~FILTER_BASICBODIES);
	Value |= Filter;
}
