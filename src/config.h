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
#ifndef CONFIG_H
#define CONFIG_H

// Load/save config file
class _Config {

public:

	void Init(const std::string &ConfigFile);
	void Close();

	void Load();
	void Save();
	void SetDefaults();

	// Graphics
	int Width, Height;
	int FSAA;
	int Aniso;
	bool Fullscreen;
	bool Vsync;

	// Game
	float TimeStep;
	float MouseX, MouseY;

private:

	template <typename Type>
	void GetValue(const std::string &Field, Type &Value) {
		MapIterator = Map.find(Field);
		if(MapIterator != Map.end()) {
			std::stringstream Stream(MapIterator->second);
			Stream >> Value;
		}
	}

	// State
	std::string ConfigFile;
	std::map<std::string, std::string> Map;
	std::map<std::string, std::string>::iterator MapIterator;
};

extern _Config Config;

#endif
