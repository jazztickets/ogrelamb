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
class btCollisionShape;
class btKinematicCharacterController;
class btPairCachingGhostObject;

// Character controller
class _Character {

	public:

		_Character(const _Spawn &Spawn, btCollisionShape *Shape);
		~_Character();

		void Update(float FrameTime);

		// Objects
		btKinematicCharacterController *Controller;
		btPairCachingGhostObject *Ghost;

	private:

};