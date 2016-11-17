/*************************************************************************************
*	ogrelamb - https://github.com/jazztickets/ogrelamb
*	Copyright (C) 2016  Alan Witkowski
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
#pragma once
#include <LinearMath/btMotionState.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

// Forward Declarations
class _Spawn;
class _Collision;
class _Terrain;
class _Character;
class _Vehicle;

namespace Ogre {
	class SceneNode;
	class Entity;
}

// Object class
class _Object : public btMotionState {

	public:

		_Object(const _Spawn &Spawn);
		~_Object();

		void Update(float FrameTime);

		void BeginFrame();
		void EndFrame();

		// Flags
		bool Deleted : 1;
		bool MassOffset : 1;

		// Graphics
		Ogre::SceneNode *Node;
		Ogre::Entity *Entity;

		// Physics
		btCollisionObject *Body;
		btTransform CenterOfMassTransform;
		btTransform GraphicsTransform;

		// Character
		_Character *Character;

		// Vehicle
		_Vehicle *Vehicle;

		// Static Collision
		_Collision *Collision;

		// Terrain
		_Terrain *Terrain;

		void getWorldTransform(btTransform &Transform) const;
		void setWorldTransform(const btTransform &Transform);

	private:

		btTransform ChildShapeOffset;

};
