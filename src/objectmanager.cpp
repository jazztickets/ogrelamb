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
#include "objectmanager.h"
#include "object.h"
#include "template.h"
#include "log.h"

_ObjectManager ObjectManager;

// Initialize
void _ObjectManager::Init() {
}

// Shutdown
void _ObjectManager::Close() {

	ClearObjects();
}

// Updates all objects
void _ObjectManager::Update(float FrameTime) {

	// Update objects
	for(std::list<_Object *>::iterator Iterator = Objects.begin(); Iterator != Objects.end(); ) {
		_Object *Object = *Iterator;

		// Update the object
		Object->BeginFrame();
		Object->Update(FrameTime);
		
		// Delete old objects
		if(Object->Deleted) {

			// Delete object
			delete Object;			
			Iterator = Objects.erase(Iterator);
		}
		else {
			
			++Iterator;
		}
	}
}

// Deletes all of the objects
void _ObjectManager::ClearObjects() {

	// Delete objects
	for(std::list<_Object *>::iterator Iterator = Objects.begin(); Iterator != Objects.end(); ++Iterator)
		delete (*Iterator);
	
	Objects.clear();
}

// Adds an object to the manager
void _ObjectManager::AddObject(_Object *Object) {

	// Store in list
	Objects.push_back(Object);
}

// Create an object from a template
_Object *_ObjectManager::CreateObject(const _Spawn &Spawn) {

	// Create and add object
	_Object *Object;
	try {
		Object = new _Object(Spawn);
		ObjectManager.AddObject(Object);
	}
	catch(Ogre::Exception &e) {
		Log << __FUNCTION__ << " -- Ogre::Exception: " << e.what() << std::endl;
		Object = NULL;
	}

	return Object;
}
