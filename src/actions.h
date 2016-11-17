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
#include <list>

// Constants
const int ACTIONS_MAX = 64;
const int ACTIONS_MAXKEYS = 256;
const int ACTIONS_MAXBUTTONS = 8;

// Handles actions
class _Actions {

	public:

		enum Types {
			Forward,
			Back,
			Left,
			Right,
			Jump,
			Sprint,
			Ascend,
			Descend,
			Fast,
			Faster,
			Vehicle_Accel,
			Vehicle_Reverse,
			Vehicle_SteerLeft,
			Vehicle_SteerRight,
			Vehicle_Brake,
			Vehicle_Flip,
			Vehicle_Boost,
		};

		_Actions();

		// Actions
		bool GetState(int Action);

		// Maps
		void AddKeyMap(int Key, int Action);
		void AddMouseMap(int Button, int Action);

		// Handlers
		void KeyEvent(int Key, bool Pressed);
		void MouseEvent(int Key, bool Pressed);

	private:

		std::list<int> KeyMap[ACTIONS_MAXKEYS];
		std::list<int> MouseMap[ACTIONS_MAXBUTTONS];
		std::list<int>::iterator MapIterator;

		bool State[ACTIONS_MAX];
};

extern _Actions Actions;
