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

#include "collision.h"
#include "template.h"
#include "framework.h"
#include "physics.h"
#include "log.h"

// Load a .col file
_Collision::_Collision(const _Spawn &Spawn) {
	
	// Open file
	std::string FilePath(std::string("collision/") + Spawn.Template->CollisionFile);
	std::ifstream File(FilePath.c_str(), std::ios::binary);
	if(!File.is_open()) {
		Log << "Error opening collision file: " << FilePath << std::endl;
		return;
	}

	// Read header
	int VertCount, FaceCount;
	File.read((char *)&VertCount, sizeof(int));
	File.read((char *)&FaceCount, sizeof(int));

	// Get data size
	int VertDataSize = VertCount * 3;
	int FaceDataSize = FaceCount * 3;

	// Allocate memory for lists
	VertexList = new float[VertDataSize];
	FaceList = new int[FaceDataSize];

	// Read vertices
	File.read((char *)VertexList, VertDataSize * sizeof(float));

	// Read faces
	File.read((char *)FaceList, FaceDataSize * sizeof(int));
	
	// Create vertex array object
	TriangleIndexVertexArray = new btTriangleIndexVertexArray(FaceCount, FaceList, 3 * sizeof(int), VertDataSize, VertexList, 3 * sizeof(float));

	File.close();
}

// Destructor
_Collision::~_Collision() {

	delete TriangleIndexVertexArray;
	delete[] VertexList;
	delete[] FaceList;
}
