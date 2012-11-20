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
#include "actions.h"

_Actions Actions;

// Constructor
_Actions::_Actions() {
	for(int i = 0; i < ACTIONS_MAX; i++)
		State[i] = 0;
}

// Get action
bool _Actions::GetState(int Action) {
	if(Action < 0 || Action >= ACTIONS_MAX)
		return false;

	return State[Action];
}

// Add to the key=>action map
void _Actions::AddKeyMap(int Key, int Action) {
	KeyMap[Key].push_back(Action);
}

// Add to the mouse button=>action map
void _Actions::AddMouseMap(int Button, int Action) {
	MouseMap[Button].push_back(Action);
}

// Handle keys
void _Actions::KeyEvent(int Key, bool Pressed) {
	if(Key < 0 || Key >= ACTIONS_MAXKEYS)
		return;

	for(MapIterator = KeyMap[Key].begin(); MapIterator != KeyMap[Key].end(); MapIterator++) {
		State[*MapIterator] = Pressed;
	}
}

// Handle mouse
void _Actions::MouseEvent(int Button, bool Pressed) {
	if(Button < 0 || Button >= ACTIONS_MAXBUTTONS)
		return;

	for(MapIterator = MouseMap[Button].begin(); MapIterator != MouseMap[Button].end(); MapIterator++) {
		State[*MapIterator] = Pressed;
	}
}
