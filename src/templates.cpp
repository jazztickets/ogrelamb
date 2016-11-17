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

#include "templates.h"
#include "template.h"

_Templates Templates;

// Initialize
void _Templates::Init() {

	{
		_Template *Template = new _Template;
		Template->MeshFile = "capsule.mesh";
		Template->Material = "crate";
		//Template->Mass = 0.25f;
		Template->ShapeType = _Template::SHAPE_CAPSULE;
		Template->Shape.setValue(1, 2, 0);
		Template->CharacterType = _Template::CHARACTER_BASIC;
		TemplateMap["character"] = Template;
	}
	{
		_Template *Template = new _Template;
		Template->MeshFile = "cube.mesh";
		Template->Material = "crate";
		Template->Mass = 0.25f;
		Template->ShapeType = _Template::SHAPE_BOX;
		TemplateMap["crate"] = Template;
	}
	{
		_Template *Template = new _Template;
		Template->MeshFile = "sphere.mesh";
		Template->Material = "crate";
		Template->Mass = 0.25f;
		Template->MassOffset.setValue(0.0f, -0.25f, 0.0f);
		Template->ShapeType = _Template::SHAPE_SPHERE;
		Template->Radius = 0.5f;
		TemplateMap["sphere"] = Template;
	}
	{
		_Template *Template = new _Template;
		Template->MeshFile = "car.mesh";
		Template->Material = "crate";
		//Template->Friction = 0.1;
		Template->VehicleType = _Template::VEHICLE_BASIC;
		Template->ShapeType = _Template::SHAPE_BOX;
		Template->Shape.setValue(2, 1, 4);
		Template->MassOffset.setValue(0.0f, -0.25f, 0.0f);
		TemplateMap["car"] = Template;
	}
	{
		_Template *Template = new _Template;
		Template->MeshFile = "room.mesh";
		Template->Material = "checker";
		Template->CollisionFile = "room.col";
		Template->CollisionType = _Template::COLLISION_STATICMESH;
		Template->ShapeType = _Template::SHAPE_TRIANGLEMESH;
		Template->Mass = 0.0f;
		Template->Scale.setValue(5, 5, 5);
		TemplateMap["room"] = Template;
	}
	{
		_Template *Template = new _Template;
		Template->MeshFile = "terrain.mesh";
		Template->Material = "grass0";
		Template->CollisionFile = "terrain.col";
		Template->CollisionType = _Template::COLLISION_STATICMESH;
		Template->ShapeType = _Template::SHAPE_TRIANGLEMESH;
		Template->Mass = 0.0f;
		Template->Scale.setValue(40, 40, 40);
		TemplateMap["terrain"] = Template;
	}
	{
		_Template *Template = new _Template;
		//Template->Material = "grass0";
		Template->TerrainType = _Template::TERRAIN_BASIC;
		//Template->HeightmapFile = "terrain.png";
		Template->ShapeType = _Template::SHAPE_HEIGHTFIELD;
		Template->Scale.setValue(2, 1, 2);
		Template->Mass = 0.0f;
		TemplateMap["terrain_ogre"] = Template;
	}
}

// Close
void _Templates::Close() {

	Clear();
}

// Clear templates
void _Templates::Clear() {

	// Delete templates
	for(_TemplateMapType::iterator Iterator = TemplateMap.begin(); Iterator != TemplateMap.end(); ++Iterator)
		delete Iterator->second;

	TemplateMap.clear();
}

// Overload [] operator to access templates easily
const _Template *_Templates::operator[](const std::string &Name) {

	return TemplateMap[Name];
}
