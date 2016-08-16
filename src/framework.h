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
#ifndef FRAMEWORK_H
#define FRAMEWORK_H

// Forward Declarations
class _State;

// Manages Ogre3D and input systems
class _Framework : public OIS::KeyListener, public OIS::MouseListener {

public:

	// Setup
	void Init(const std::string &Title);
	void Close();

	// Update functions
	void ChangeState(_State *RequestedState);
	void Update();
	void Render();

	// Input callbacks
	bool keyPressed(const OIS::KeyEvent &Event);
	bool keyReleased(const OIS::KeyEvent &Event);
	bool mouseMoved(const OIS::MouseEvent &Event);
	bool mousePressed(const OIS::MouseEvent &Event, OIS::MouseButtonID Button);
	bool mouseReleased(const OIS::MouseEvent &Event, OIS::MouseButtonID Button);

	// Graphics
	Ogre::Root *Root;
	Ogre::SceneManager *Scene;
	Ogre::RenderWindow *Window;
	Ogre::Camera *Camera;
	Ogre::ulong WindowHandle;
	Ogre::TerrainGlobalOptions *TerrainGlobalOptions;

	// Input
	OIS::InputManager *InputManager;
	OIS::Mouse *Mouse;
	OIS::Keyboard *Keyboard;

	// State
	bool Done;
	float TimeStep;
	Ogre::ulong FrameNumber;
	
private:

	void Delay(float Time);

	// States
	_State *State, *RequestedState;

	// Time
	Ogre::Timer Timer;
	Ogre::uint TimeStamp;

	// Physics
	float TimeStepAccumulator;

};

extern _Framework Game;

#endif