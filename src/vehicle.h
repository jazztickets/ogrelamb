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

// Forward Declarations
class _Spawn;
class btCollisionObject;
class btRaycastVehicle;
class btDefaultVehicleRaycaster;
namespace Ogre {
	class SceneNode;
	class Entity;
}

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
