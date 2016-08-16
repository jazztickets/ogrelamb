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
#ifndef CAMERA_H
#define CAMERA_H

// Forward Declarations
class _Object;

// Camera class
class _Camera {

public:

	enum CameraType {
		FREEMOVE,
		THIRD_PERSON
	};

	_Camera();

	// Update
	void Update(float FrameTime);
	void UpdateRender(float BlendFactor);
	void HandleMove(const btVector3 &Direction, float Speed);
	void HandleMouse(int UpdateX, int UpdateY);

	void SetPosition(const btVector3 &Position) { LastPosition = this->Position = Position; }

	int Type;
	_Object *FollowObject;
	float Sensitivity[2];
	float Distance;
	float Yaw, Pitch;
	float PitchLimit;

private:

	// State
	btVector3 Position, LastPosition, Velocity, Offset;
	
};

extern _Camera Camera;

#endif