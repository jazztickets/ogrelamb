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
#include <list>

// Forward Declarations
class _Object;
class _Spawn;

// Manages all the objects
class _ObjectManager {

	public:

		void Init();
		void Close();

		// Updates
		void Update(float FrameTime);
		void Draw();

		// Management
		void AddObject(_Object *Object);
		_Object *CreateObject(const _Spawn &Spawn);
		void ClearObjects();

	private:

		// Objects
		std::list<_Object *> Objects;
};

extern _ObjectManager ObjectManager;

