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
#ifndef PLAY_H
#define PLAY_H

// Libraries
#include "state.h"

// Forward Declarations
class _Object;

// Main game state
class _PlayState : public _State {

public:

	void Init();
	void Close();

	void KeyEvent(int Key, bool Pressed);
	void MouseMoved(int UpdateX, int UpdateY);
	void MouseEvent(int Button, bool Pressed);

	void Update(float FrameTime);
	void UpdateRender(float TimeStepRemainder, float TimeStep);
	void Draw();

private:

	_Object *Player, *Car, *Character;

};

extern _PlayState PlayState;

#endif
