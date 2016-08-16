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
#include <all.h>
#ifndef PHYSICS_H
#define PHYSICS_H

// Custom world that overrides stepSimulation for applications that implement their own fixed time step calculations
class btFixedWorld : public btDiscreteDynamicsWorld {

public:

	btFixedWorld(btDispatcher *dispatcher, btBroadphaseInterface *pairCache, btConstraintSolver *constraintSolver, btCollisionConfiguration *collisionConfiguration)
		: btDiscreteDynamicsWorld(dispatcher, pairCache, constraintSolver, collisionConfiguration) { }
	~btFixedWorld() { }

	void setTimeStepRemainder(btScalar time) { m_localTime = time; }
	int stepSimulation(btScalar timeStep, int maxSubSteps=0, btScalar fixedTimeStep=0) {
		
		saveKinematicState(timeStep);
		applyGravity();
		internalSingleStepSimulation(timeStep);
		clearForces();
		
		return 1;
	}

};

// Manages bullet physics
class _Physics {

public:

	enum FilterTypes {
		FILTER_NONE		= 0x0,
		FILTER_RIGIDBODY	= 0x1,
		FILTER_STATIC		= 0x2,
		FILTER_KINEMATIC	= 0x4,
		FILTER_CAMERA		= 0x8,
		FILTER_ZONE		= 0x10,
		FILTER_BASICBODIES	= FILTER_RIGIDBODY | FILTER_STATIC | FILTER_KINEMATIC,
	};

	void Init();
	void Close();

	void Update(float FrameTime);
	void Reset();

	// Utility
	bool RaycastWorld(const btVector3 &Start, btVector3 &End, btVector3 &ormal);
	void RemoveFilter(int &Value, int Filter);
	void SetBodyType(int &Value, int Filter);

	// Flags
	bool Enabled;

	// State
	btFixedWorld *World;
	btDefaultCollisionConfiguration *CollisionConfiguration;
	btCollisionDispatcher *Dispatcher;
	btBroadphaseInterface *BroadPhase;
	btSequentialImpulseConstraintSolver *Solver;
	btGhostPairCallback *GhostPair;

private:

};

extern _Physics Physics;

#endif
