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
#include <stdint.h>

// Forward Declarations
class _Spawn;
namespace Ogre {
	class TerrainGroup;
}

// Terrain
class _Terrain {

	public:

		_Terrain(const _Spawn &Spawn);
		~_Terrain();

		uint16_t Size;
		float *BulletHeight;
		Ogre::TerrainGroup *TerrainGroup;

	private:

		void CreateProceduralHeight(int SlotX, int SlotY, int Size, float *Data);
		void CreateBulletHeightData(const float *Data);

};
