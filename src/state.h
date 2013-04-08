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
#ifndef STATE_H
#define STATE_H

// Game state class
class _State {

public:

	// Setup
	virtual void Init() { };
	virtual void Close() { };

	// Input
	virtual void KeyEvent(int Key, bool Pressed) { };
	virtual void MouseMoved(int UpdateX, int UpdateY) { };
	virtual void MouseEvent(int Button, bool Pressed) { };

	// Update
	virtual void Update(float FrameTime) { };
	virtual void UpdateRender(float TimeStepRemainder, float TimeStep) { };
	virtual void Draw() { };

	float Time;
	unsigned long UpdateCount;

private:

};

#endif
