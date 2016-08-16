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
#include "vehicle.h"
#include "template.h"
#include "framework.h"
#include "physics.h"

// Vehicle constructor
_Vehicle::_Vehicle(const _Spawn &Spawn, btCollisionObject *Chassis) {
	SteeringLimit = 0.8f;
	ReleaseRate = 1.5f;
	Steering = 0.0f;
	SteerRate = 0.0f;
	SteerMode = 0;

	btRaycastVehicle::btVehicleTuning Tuning;
	//Tuning.m_suspensionStiffness = 10;

	VehicleRayCaster = new btDefaultVehicleRaycaster(Physics.World);
	RaycastVehicle = new btRaycastVehicle(Tuning, (btRigidBody *)Chassis, VehicleRayCaster);
	Physics.World->addVehicle(RaycastVehicle);
	RaycastVehicle->setCoordinateSystem(0, 1, 2);

	float Radius = 0.5f;
	float SuspensionLength = 0.2f;
	btVector3 WheelPoint(0.75, -0.0f, 1.50);
	RaycastVehicle->addWheel(btVector3(-WheelPoint.x(), WheelPoint.y(), WheelPoint.z()), btVector3(0, -1, 0), btVector3(-1, 0, 0), SuspensionLength, Radius, Tuning, true);
	RaycastVehicle->addWheel(btVector3(WheelPoint.x(), WheelPoint.y(), WheelPoint.z()), btVector3(0, -1, 0), btVector3(-1, 0, 0), SuspensionLength, Radius, Tuning, true);
	RaycastVehicle->addWheel(btVector3(-WheelPoint.x(), WheelPoint.y(), -WheelPoint.z()), btVector3(0, -1, 0), btVector3(-1, 0, 0), SuspensionLength, Radius, Tuning, false);
	RaycastVehicle->addWheel(btVector3(WheelPoint.x(), WheelPoint.y(), -WheelPoint.z()), btVector3(0, -1, 0), btVector3(-1, 0, 0), SuspensionLength, Radius, Tuning, false);

	for(int i = 0; i < 4; i++) {
		btWheelInfo &Info = RaycastVehicle->getWheelInfo(i);
		Info.m_suspensionStiffness = 15;
		WheelEntity[i] = Game.Scene->createEntity("wheel.mesh");
		WheelEntity[i]->setMaterialName("crate");
		WheelNode[i] = Game.Scene->getRootSceneNode()->createChildSceneNode();
		WheelNode[i]->attachObject(WheelEntity[i]);
		//Node->scale(Spawn.Template->Scale, Spawn.Template->Scale, Spawn.Template->Scale);
	}
}

// Vehicle destructor
_Vehicle::~_Vehicle() {

	for(int i = 0; i < 4; i++) {
		Game.Scene->destroyEntity(WheelEntity[i]);
		Game.Scene->destroySceneNode(WheelNode[i]);
	}

	Physics.World->removeVehicle(RaycastVehicle);

	delete VehicleRayCaster;
	delete RaycastVehicle;
}

// Update the vehicle
void _Vehicle::Update(float FrameTime) {

	if(SteerMode) {
		Steering += SteerRate;
	}
	else {
		Steering /= ReleaseRate;
	}
	btClamp<float>(Steering, -SteeringLimit, SteeringLimit);
	
	RaycastVehicle->setSteeringValue(Steering, 0);
	RaycastVehicle->setSteeringValue(Steering, 1);
}

// Set steer rate
void _Vehicle::SetSteer(float Rate) {
	SteerRate = Rate;
	SteerMode = 1;
}

// Let go of steering
void _Vehicle::ReleaseSteer(float Rate) {
	SteerMode = 0;
	ReleaseRate = Rate;
}
