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

#include "config.h"
#include "log.h"

_Config Config;

// Initializes the config system
void _Config::Init(const std::string &ConfigFile) {
	this->ConfigFile = ConfigFile;

	// Load defaults
	SetDefaults();
	
	// Load config
	Load();
}

// Closes the config system
void _Config::Close() {
	Save();
}

// Set defaults
void _Config::SetDefaults() {

	Width = 800;
	Height = 600;
	FSAA = 0;
	Aniso = 0;
	Fullscreen = false;
	Vsync = false;

	TimeStep = 0.01f;
	MouseX = 0.4f;
	MouseY = -0.4f;
}

// Load the config file
void _Config::Load() {
	
	// Open file
	std::ifstream In(ConfigFile.c_str());
	if(!In.is_open()) {
		Log << __FUNCTION__ << " -- Config file not found, creating new one: " << ConfigFile << std::endl;
		Save();
		return;
	}

	// Read data into map
	Map.clear();
	char Buffer[256];
	while(In) {

		In.getline(Buffer, 256);
		if(In.good()) {
			std::string Line(Buffer);
			std::size_t Pos = Line.find_first_of('=');
			if(Pos != std::string::npos) {
				std::string Field = Line.substr(0, Pos);
				std::string Value = Line.substr(Pos+1, Line.size());

				Map[Field] = Value;
			}
		}
	}
	In.close();

	// Read config
	GetValue("width", Width);
	GetValue("height", Height);
	GetValue("fullscreen", Fullscreen);
	GetValue("vsync", Vsync);
	GetValue("aniso", Aniso);
	GetValue("fsaa", FSAA);
	GetValue("mousex", MouseX);
	GetValue("mousey", MouseY);
}

// Save variables to the config file
void _Config::Save() {

	std::ofstream Out(ConfigFile.c_str());
	if(!Out.is_open()) {
		Log << __FUNCTION__ << " -- Error opening config file for saving: " << ConfigFile << std::endl;
		return;
	}

	// Write variables
	Out << "width=" << Width << std::endl;
	Out << "height=" << Height << std::endl;
	Out << "fullscreen=" << Fullscreen << std::endl;
	Out << "vsync=" << Vsync << std::endl;
	Out << "fsaa=" << FSAA << std::endl;
	Out << "aniso=" << Aniso << std::endl;
	Out << "mousex=" << MouseX << std::endl;
	Out << "mousey=" << MouseY << std::endl;

	Out.close();
}
