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

#include "template.h"
#include "object.h"
#include "physics.h"

// Default constructor
_Template::_Template() {

	// Graphics
	MeshFile = "";
	Material = "";
	Scale.setValue(1.0f, 1.0f, 1.0f);

	// Physics
	ShapeType = SHAPE_NONE;
	Shape.setValue(1.0f, 1.0f, 1.0f);
	MassOffset.setValue(0.0f, 0.0f, 0.0f);
	Radius = 0.5f;
	Mass = 1.0f;
	Friction = 1.0f;
	Restitution = 0.2f;
	LinearDamping = 0.1f;
	AngularDamping = 0.3f;

	// Character
	int CharacterType = CHARACTER_NONE;

	// Vehicle
	int VehicleType = VEHICLE_NONE;

	// Collision
	int CollisionType = COLLISION_NONE;
	CollisionFile = "";

	// Terrain
	int TerrainType = TERRAIN_NONE;
	HeightmapFile = "";	
}

// Default constructor
_Spawn::_Spawn() {
	Name = "";
	Position.setValue(0.0f, 0.0f, 0.0f);
	Rotation.setEuler(0.0f, 0.0f, 0.0f);
	Template = NULL;
}

// Constructor
_Spawn::_Spawn(const _Template *Template, const std::string &Name, const btVector3 &Position, const btQuaternion &Rotation) {
	this->Name = Name;
	this->Template = Template;
	this->Position = Position;
	this->Rotation = Rotation;
}
