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
#ifndef VEHICLE_H
#define VEHICLE_H

// Forward Declarations
class _Spawn;

// Vehicle
class _Vehicle {

public:

	_Vehicle(const _Spawn &Spawn, btCollisionObject *Chassis);
	~_Vehicle();

	void Update(float FrameTime);
	void SetSteer(float Rate);
	void ReleaseSteer(float Rate);

	btRaycastVehicle *RaycastVehicle;
	Ogre::SceneNode *WheelNode[4];
	Ogre::Entity *WheelEntity[4];

private:

	int SteerMode;
	float Steering, SteerRate, SteeringLimit, ReleaseRate;

	btDefaultVehicleRaycaster *VehicleRayCaster;
};

#endif