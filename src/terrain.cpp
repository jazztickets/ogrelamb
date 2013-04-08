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
#include "terrain.h"
#include "template.h"
#include "framework.h"
#include "physics.h"
#include "log.h"

// Constructor
_Terrain::_Terrain(const _Spawn &Spawn) {
	Size = 129;
	BulletHeight = NULL;

	TerrainGroup = new Ogre::TerrainGroup(Game.Scene, Ogre::Terrain::ALIGN_X_Z, Size, 256);
	TerrainGroup->setFilenameConvention("terrain", "dat");
	TerrainGroup->setOrigin(Ogre::Vector3(0, 0, 0));

	Ogre::Terrain::ImportData &DefaultSettings = TerrainGroup->getDefaultImportSettings();
	DefaultSettings.layerList.resize(1);
	DefaultSettings.layerList[0].worldSize = 10;
	DefaultSettings.layerList[0].textureNames.push_back("grass0.jpg");
	DefaultSettings.layerList[0].textureNames.push_back("normal.png");

	if(Ogre::ResourceGroupManager::getSingleton().resourceExists(TerrainGroup->getResourceGroup(), TerrainGroup->generateFilename(0, 0))) {
		TerrainGroup->defineTerrain(0, 0);
	}
	else {
		float *OgreHeight = new float[Size * Size];
		CreateProceduralHeight(0, 0, Size, OgreHeight);
		TerrainGroup->defineTerrain(0, 0, OgreHeight);
		delete[] OgreHeight;
	}
	TerrainGroup->loadAllTerrains(true);
	TerrainGroup->freeTemporaryResources();
	CreateBulletHeightData(TerrainGroup->getTerrain(0, 0)->getHeightData());
}

// Destructor
_Terrain::~_Terrain() {
	TerrainGroup->saveAllTerrains(true);
	delete TerrainGroup;
	delete[] BulletHeight;
}

// Create a copy of the height map data and reflect about x-axis for bullet
void _Terrain::CreateBulletHeightData(const float *Data) {
	if(BulletHeight)
		return;

	BulletHeight = new float[Size * Size];
	for(Ogre::uint16 i = 0; i < Size; i++) {
		memcpy(BulletHeight + Size * i, Data + (Size - i - 1) * Size, sizeof(float) * Size);
	}
}

// Create some terrain
void _Terrain::CreateProceduralHeight(int SlotX, int SlotY, int Size, float *Data) {
	for(int i = 0; i < Size; i++) {
		for(int j = 0; j < Size; j++) {
			float x = float(j) / (float)Size;
			float y = float(i) / (float)Size;
			Data[i * Size + j] = 10 * cos(x * Ogre::Math::PI * 4) * sin(y * Ogre::Math::PI * 4);
		}
	}
}
