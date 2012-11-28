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
#include "terrain.h"
#include "template.h"
#include "framework.h"
#include "physics.h"
#include "log.h"

// Constructor
_Terrain::_Terrain(const _Spawn &Spawn) {
	
	Size = 129;
	HeightData = new float[Size * Size];
	for(int i = 0; i < Size; i++) {
		for(int j = 0; j < Size; j++) {
			float gradx = float(j) / (float)Size;
			float grady = float(i) / (float)Size;
			HeightData[(Size - i - 1) * Size + j] = 5 * cos(gradx * Ogre::Math::PI * 4) * sin(grady * Ogre::Math::PI * 4);
			//Height[i * Size + j] = 0;//cos(gradx * Ogre::Math::PI * 4) * sin(grady * Ogre::Math::PI * 4);
		}
	}

}

// Destructor
_Terrain::~_Terrain() {

	delete[] HeightData;
}
