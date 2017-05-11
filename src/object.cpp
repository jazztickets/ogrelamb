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

#include "object.h"
#include "template.h"
#include "framework.h"
#include "physics.h"
#include "character.h"
#include "vehicle.h"
#include "collision.h"
#include "terrain.h"
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Vehicle/btRaycastVehicle.h>

// Constructor
_Object::_Object(const _Spawn &Spawn) {
	Deleted = 0;
	MassOffset = 0;
	Node = NULL;
	Entity = NULL;
	Body = NULL;
	Character = NULL;
	Vehicle = NULL;
	Collision = NULL;
	Terrain = NULL;

	const _Template *Template = Spawn.Template;

	// Create graphics node
	if(!Template->MeshFile.empty()) {
		Entity = Game.Scene->createEntity(Template->MeshFile);
		if(Template->Material != "")
			Entity->setMaterialName(Template->Material);

		Node = Game.Scene->getRootSceneNode()->createChildSceneNode();
		Node->attachObject(Entity);
		Node->scale(Template->Scale.x(), Template->Scale.y(), Template->Scale.z());
	}

	// Collision type
	switch(Template->CollisionType) {
		case _Template::COLLISION_STATICMESH:
			Collision = new _Collision(Spawn);
		break;
	}

	// Terrain type
	switch(Template->TerrainType) {
		case _Template::TERRAIN_BASIC:
			Terrain = new _Terrain(Spawn);
		break;
	}

	// Set up shape
	btCollisionShape *BasicShape = NULL;
	btVector3 HalfExtents = Template->Shape * 0.5f;
	switch(Template->ShapeType) {
		case _Template::SHAPE_BOX:
			BasicShape = new btBoxShape(HalfExtents);
		break;
		case _Template::SHAPE_SPHERE:
			BasicShape = new btSphereShape(Template->Radius);
		break;
		case _Template::SHAPE_CYLINDER:
			BasicShape = new btCylinderShape(HalfExtents);
		break;
		case _Template::SHAPE_CAPSULE:
			BasicShape = new btCapsuleShape(Template->Shape.x() * 0.5f, Template->Shape.y());
		break;
		case _Template::SHAPE_TRIANGLEMESH:
			if(Collision)
				BasicShape = new btBvhTriangleMeshShape(Collision->TriangleIndexVertexArray, true);
		break;
		case _Template::SHAPE_HEIGHTFIELD:
			if(Terrain) {
				btHeightfieldTerrainShape *HeightField = new btHeightfieldTerrainShape(Terrain->Size, Terrain->Size, Terrain->BulletHeight, 0, -50, 50, 1, PHY_FLOAT, true);
				HeightField->setUseDiamondSubdivision(true);
				BasicShape = HeightField;
			}
		break;
	}

	// Handle center of mass offset
	btCollisionShape *Shape = NULL;
	ChildShapeOffset.setIdentity();
	if(!Template->MassOffset.fuzzyZero()) {

		// Child Position = -MassOffset
		ChildShapeOffset.setOrigin(Template->MassOffset * -1);
		btCompoundShape *CompoundShape = new btCompoundShape();
		CompoundShape->addChildShape(ChildShapeOffset, BasicShape);

		MassOffset = 1;
		Shape = CompoundShape;
	}
	else
		Shape = BasicShape;

	// Set scaling
	if(Shape)
		Shape->setLocalScaling(btVector3(Template->Scale.x(), Template->Scale.y(), Template->Scale.z()));

	// Spawn the object using Spawn.Position as the graphics transform
	CenterOfMassTransform.setIdentity();
	CenterOfMassTransform.setOrigin(Spawn.Position);
	CenterOfMassTransform.setRotation(Spawn.Rotation);
	CenterOfMassTransform *= ChildShapeOffset.inverse();

	// Rigid body needs a shape
	if(Shape && Template->CharacterType != _Template::CHARACTER_BASIC) {

		// Local inertia
		btVector3 LocalInertia(0.0f, 0.0f, 0.0f);
		if(Template->Mass != 0.0f)
			Shape->calculateLocalInertia(Template->Mass, LocalInertia);

		// Create body
		btRigidBody *RigidBody = new btRigidBody(Template->Mass, this, Shape, LocalInertia);
		RigidBody->setUserPointer((void *)this);
		RigidBody->setFriction(Template->Friction);
		RigidBody->setDamping(Template->LinearDamping, Template->AngularDamping);
		RigidBody->setSleepingThresholds(0.2f, 0.2f);
		Body = RigidBody;

		// Add body
		//Physics.World->addRigidBody(RigidBody, Template->CollisionGroup, Template->CollisionMask);
		Physics.World->addRigidBody(RigidBody);
	}

	// Set up character
	switch(Template->CharacterType) {
		case _Template::CHARACTER_BASIC: {
			Character = new _Character(Spawn, Shape);
		} break;
	}

	// Set up vehicle
	switch(Template->VehicleType) {
		case _Template::VEHICLE_BASIC: {
			Vehicle = new _Vehicle(Spawn, Body);
			Body->setActivationState(DISABLE_DEACTIVATION);
		} break;
	}
}

// Destructor
_Object::~_Object() {
	if(Entity)
		Game.Scene->destroyEntity(Entity);

	if(Node)
		Game.Scene->destroySceneNode(Node);

	if(Body) {
		Physics.World->removeCollisionObject(Body);
		if(Body->getCollisionShape()->getShapeType() == COMPOUND_SHAPE_PROXYTYPE) {
			btCompoundShape *CompoundShape = (btCompoundShape *)Body->getCollisionShape();
			for(int i = 0; i < CompoundShape->getNumChildShapes(); i++)
				delete CompoundShape->getChildShape(i);
		}
		delete Body->getCollisionShape();
		delete Body;
	}

	if(Character)
		delete Character;

	if(Vehicle)
		delete Vehicle;

	if(Collision)
		delete Collision;

	if(Terrain)
		delete Terrain;
}

// Update the object
void _Object::Update(float FrameTime) {

	if(Character) {
		Character->Update(FrameTime);
		setWorldTransform(Character->Ghost->getWorldTransform());
	}

	if(Vehicle)
		Vehicle->Update(FrameTime);
}

// Get the center of mass transform for the object
void _Object::getWorldTransform(btTransform &Transform) const {
	Transform = CenterOfMassTransform;
}

// Set the center of mass transform for the object
void _Object::setWorldTransform(const btTransform &Transform) {
	CenterOfMassTransform = Transform;

	// Get the graphics transform
	GraphicsTransform = CenterOfMassTransform * ChildShapeOffset;

	// Set position
	const btVector3 &Position = GraphicsTransform.getOrigin();
	Node->setPosition(Position.x(), Position.y(), Position.z());

	// Set rotation
	const btQuaternion &Rotation = GraphicsTransform.getRotation();
	Node->setOrientation(Rotation.getW(), Rotation.getX(), Rotation.getY(), Rotation.getZ());
	if(Vehicle) {
		for(int i = 0; i < Vehicle->RaycastVehicle->getNumWheels(); i++) {
			Vehicle->RaycastVehicle->updateWheelTransform(i, true);
			btTransform &WheelTransform = Vehicle->RaycastVehicle->getWheelInfo(i).m_worldTransform;

			// Set wheel transform
			Vehicle->WheelNode[i]->setPosition(WheelTransform.getOrigin().x(), WheelTransform.getOrigin().y(), WheelTransform.getOrigin().z());
			Vehicle->WheelNode[i]->setOrientation(WheelTransform.getRotation().getW(), WheelTransform.getRotation().getX(), WheelTransform.getRotation().getY(), WheelTransform.getRotation().getZ());
		}
	}
}

// Function called before update
void _Object::BeginFrame() {
}

// Function called after update
void _Object::EndFrame() {
}
