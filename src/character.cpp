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
#include "character.h"
#include "template.h"
#include "framework.h"
#include "physics.h"

// Character constructor
_Character::_Character(const _Spawn &Spawn, btCollisionShape *Shape) {

	// Create ghost collision object
	Ghost = new btPairCachingGhostObject();
	Ghost->setCollisionShape(Shape);
	Ghost->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	
	// TODO cleanup
	btTransform World;
	World.setIdentity();
	World.setOrigin(Spawn.Position);
	Ghost->setWorldTransform(World);
	
	// Create character controller
	Controller = new btKinematicCharacterController(Ghost, (btConvexShape *)Shape, 0.35f);
	Physics.World->addCollisionObject(Ghost, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	Physics.World->addAction(Controller);
}

// Character destructor
_Character::~_Character() {
	Physics.World->removeAction(Controller);
	Physics.World->removeCollisionObject(Ghost);

	delete Controller;
	delete Ghost->getCollisionShape();
	delete Ghost;
}

// Update the Character
void _Character::Update(float FrameTime) {

}
