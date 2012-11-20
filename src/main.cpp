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
#include "framework.h"
#include "templates.h"
#include "play.h"
#include "config.h"
#include "log.h"

int main(int ArgumentCount, char **Arguments) {

	// Open log file
	Log.Open("game.log");

	// Load config file
	Config.Init("game.cfg");

	// Init framework
	Game.Init("ogrelamb");

	// Init system
	Templates.Init();

	// First state
	Game.ChangeState(&PlayState);

	// Main loop
	while(!Game.Done) {

		// Update application
		Game.Update();
	}

	// Shutdown
	Templates.Close();
	Config.Close();

	// Close framework
	Game.Close();

	return 0;
}
