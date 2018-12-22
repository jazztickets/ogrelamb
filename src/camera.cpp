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

#include "camera.h"
#include "object.h"
#include "framework.h"
#include <OgreCamera.h>

_Camera Camera;

// Constructor
_Camera::_Camera() {
	FollowObject = NULL;
	Type = FREEMOVE;
	Position.setZero();
	LastPosition.setZero();
	Offset.setZero();
	Velocity.setZero();
	Yaw = 0.0f;
	Pitch = 0.0f;
	Distance = 5.0f;

	PitchLimit = btRadians(85);
	Sensitivity[0] = 0.4f;
	Sensitivity[1] = 0.4f;
}

// Update the object
void _Camera::Update(float FrameTime) {
	switch(Type) {
		case FREEMOVE:
			LastPosition = Position;
			Position += Velocity;
		break;
	}
}

// Update the camera
void _Camera::UpdateRender(float BlendFactor) {

	switch(Type) {
		case FREEMOVE: {
			btVector3 InterpolatedPosition = Position * BlendFactor + LastPosition * (1.0f - BlendFactor);
			Game.Camera->setPosition(Ogre::Vector3(InterpolatedPosition.x(), InterpolatedPosition.y(), InterpolatedPosition.z()));
		} break;
		case THIRD_PERSON: {
			if(FollowObject) {
				btVector3 NewPosition = FollowObject->GraphicsTransform.getOrigin() + Offset;
				Game.Camera->setPosition(Ogre::Vector3(NewPosition.x(), NewPosition.y(), NewPosition.z()));
			}
		} break;
	}
}

// Movement for free moving cameras
void _Camera::HandleMove(const btVector3 &Direction, float Speed) {
	switch(Type) {
		case FREEMOVE: {
			if(!Direction.isZero()) {

				btVector3 Move(Direction);
				Move.normalize();
				Move *= Speed;
				btQuaternion Orientation(Game.Camera->getDerivedOrientation().x, Game.Camera->getDerivedOrientation().y, Game.Camera->getDerivedOrientation().z, Game.Camera->getDerivedOrientation().w);
				Velocity = quatRotate(Orientation, Move);
			}
			else
				Velocity.setZero();
		} break;
	}
}

// Handle mouse movement
void _Camera::HandleMouse(int UpdateX, int UpdateY) {

	Yaw += btRadians((btScalar)UpdateX) * Sensitivity[0];
	Pitch += btRadians((btScalar)UpdateY) * Sensitivity[1];
	if(Pitch > PitchLimit)
		Pitch = PitchLimit;
	else if(Pitch < -PitchLimit)
		Pitch = -PitchLimit;

	float AdjustedYaw = Yaw - SIMD_HALF_PI;
	float AdjustedPitch = Pitch + SIMD_HALF_PI;
	float CosPitch = cosf(AdjustedPitch);
	float SinPitch = sinf(AdjustedPitch);
	float CosYaw = cosf(AdjustedYaw);
	float SinYaw = sinf(AdjustedYaw);

	btVector3 Direction(SinPitch * CosYaw, CosPitch, SinPitch * SinYaw);
	switch(Type) {
		case FREEMOVE: {
			Game.Camera->setDirection(Ogre::Vector3(Direction.x(), Direction.y(), Direction.z()));
		} break;
		case THIRD_PERSON: {
			Game.Camera->setDirection(Ogre::Vector3(Direction.x(), Direction.y(), Direction.z()));
			Offset = Direction * -Distance;
		} break;
	}
}
