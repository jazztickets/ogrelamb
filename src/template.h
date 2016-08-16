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
#ifndef TEMPLATE_H
#define TEMPLATE_H

// Template class that describes an object type
class _Template {
public:

	enum ShapeType {
		SHAPE_NONE,
		SHAPE_BOX,
		SHAPE_SPHERE,
		SHAPE_CYLINDER,
		SHAPE_CAPSULE,
		SHAPE_COMPOUND,
		SHAPE_TRIANGLEMESH,
		SHAPE_HEIGHTFIELD,
	};

	enum CharacterType {
		CHARACTER_NONE,
		CHARACTER_BASIC
	};

	enum VehicleType {
		VEHICLE_NONE,
		VEHICLE_BASIC
	};

	enum CollisionType {
		COLLISION_NONE,
		COLLISION_STATICMESH,
	};

	enum TerrainType {
		TERRAIN_NONE,
		TERRAIN_BASIC,
	};

	_Template();

	// Graphics
	std::string MeshFile, Material;
	btVector3 Scale;
	
	// Physics
	int ShapeType;
	btVector3 Shape;
	btVector3 MassOffset;
	float Radius;
	float Mass, Friction, Restitution;
	float LinearDamping, AngularDamping;

	// Character
	int CharacterType;

	// Vehicle
	int VehicleType;

	// Collision
	int CollisionType;
	std::string CollisionFile;

	// Terrain
	int TerrainType;
	std::string HeightmapFile;

};

// Spawn class that describes object instances
class _Spawn {
public:

	_Spawn();
	_Spawn(const _Template *Template, const std::string &Name, const btVector3 &Position=btVector3(0, 0, 0), const btQuaternion &Rotation=btQuaternion(0, 0, 0));

	std::string Name;
	btVector3 Position;
	btQuaternion Rotation;
	const _Template *Template;
};

#endif
