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
#include "config.h"
#include "log.h"
#include "state.h"

// Global instance
_Framework Game;

// Initialize
void _Framework::Init(const std::string &Title) {
	Done = true;
	TimeStepAccumulator = 0.0f;
	TimeStamp = 0;
	FrameNumber = 0;
	State = RequestedState = NULL;
	TimeStep = Config.TimeStep;
	
	// Create the ogre root
	Root = new Ogre::Root();

	// Load plugins
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		#if OGRE_DEBUG_MODE == 1
			Root->loadPlugin("RenderSystem_GL_d.dll");
			Root->loadPlugin("Plugin_CgProgramManager_d.dll");
			//Root->loadPlugin("RenderSystem_Direct3D9_d.dll");
		#else
			Root->loadPlugin("RenderSystem_GL.dll");
			Root->loadPlugin("Plugin_CgProgramManager.dll");
			//Root->loadPlugin("RenderSystem_Direct3D9.dll");
		#endif
	#else
		Root->loadPlugin("/usr/local/lib/OGRE/RenderSystem_GL.so");
		Root->loadPlugin("/usr/local/lib/OGRE/Plugin_CgProgramManager.so");
	#endif

	// Get renderer
	const Ogre::RenderSystemList &RenderSystemList = Root->getAvailableRenderers();
	if(RenderSystemList.size() == 0) {
		Ogre::LogManager::getSingleton().logMessage("Sorry, no rendersystem was found.");
		return;
	}
	Ogre::RenderSystem *RenderSystem = RenderSystemList[0];
	Root->setRenderSystem(RenderSystem);

	// Initialize ogre
	Root->initialise(false);

	// Window parameters
	Ogre::NameValuePairList WindowParameters;
	{
		std::stringstream StringStream;
		StringStream << Config.FSAA;
		WindowParameters["FSAA"] = StringStream.str();
	}
	if(Config.Vsync)
		WindowParameters["vsync"] = "true";

	// Create a window
	Window = Root->createRenderWindow(Title, Config.Width, Config.Height, Config.Fullscreen, &WindowParameters);
	Window->getCustomAttribute("WINDOW", &WindowHandle);

	// Set anisotropic filtering
	if(Config.Aniso) {
		Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::FO_ANISOTROPIC, Ogre::FO_ANISOTROPIC, Ogre::FO_ANISOTROPIC);
		Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(Config.Aniso);
	}

	// Set path for resources
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("meshes", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Create scene manager
	Scene = Root->createSceneManager(Ogre::ST_GENERIC);
	//Scene->setAmbientLight(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
	Scene->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	//Scene->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	Scene->setShadowTechnique(Ogre::SHADOWTYPE_NONE);	

	// Create camera
	Camera = Scene->createCamera("main");
	Camera->setPosition(Ogre::Vector3(0.0f, 0.0f, 0.0f));
	Camera->lookAt(Ogre::Vector3(0.0f, 0.0f, -1.0f));
	Camera->setNearClipDistance(0.1f);
	Camera->setLodBias(1.0f);
	if(Root->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
		Camera->setFarClipDistance(0.0f);
	else
		Camera->setFarClipDistance(5000.0f);
	Camera->setAspectRatio((float)Config.Width / Config.Height);

	// Set up viewport
	Ogre::Viewport *Viewport = Window->addViewport(Camera);
	Viewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));

	// Create input managers
	OIS::ParamList ParameterList;
	ParameterList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(WindowHandle)));
	InputManager = OIS::InputManager::createInputSystem(ParameterList);
	Mouse = static_cast<OIS::Mouse *>(InputManager->createInputObject(OIS::OISMouse, true));
	Keyboard = static_cast<OIS::Keyboard *>(InputManager->createInputObject(OIS::OISKeyboard, true));

	// Set callbacks
	Mouse->setEventCallback(this);
	Keyboard->setEventCallback(this);

	// Set mouse limits
	Mouse->getMouseState().width = Config.Width;
	Mouse->getMouseState().height = Config.Height;

	// Restart timer
	Timer.reset();
	Done = false;
}

// Shutdown
void _Framework::Close() {
	
	// Close the current state
	if(State)
		State->Close();

	// Shutdown Ogre
	OIS::InputManager::destroyInputSystem(InputManager);
	delete Root;
}

// Update input
void _Framework::Update() {

	// Handle last frame time
	float FrameTime = (Timer.getMilliseconds() - TimeStamp) * 0.001f;
	TimeStamp = Timer.getMilliseconds();
	if(FrameTime > 1.0f)
		FrameTime = 1.0f;

	// Limit frame rate
	float ExtraTime = TimeStep - FrameTime;
	if(ExtraTime > 0.0f)
		Delay(ExtraTime);
				
	// Read input
	Ogre::WindowEventUtilities::messagePump();
	Mouse->capture();
	Keyboard->capture();

	// Accumulate time and apply fixed timestep
	TimeStepAccumulator += FrameTime;
	while(TimeStepAccumulator >= TimeStep) {

		// Update game logic
		State->Update(TimeStep);

		// Adjust accumulator
		TimeStepAccumulator -= TimeStep;
	}

	// Synchronize graphics state
	State->UpdateRender(TimeStepAccumulator, TimeStep);

	// Draw frame
	State->Draw();
	Root->renderOneFrame();

	// Check for window closure
	if(Window->isClosed())
		Done = true;

	// Update render frame count
	FrameNumber++;
}

// Change states
void _Framework::ChangeState(_State *RequestedState) {
	
	if(State)
		State->Close();

	State = RequestedState;

	if(State)
		State->Init();
}

// Mouse has been moved
bool _Framework::mouseMoved(const OIS::MouseEvent &Event) {
	if(State)
		State->MouseMoved(Event.state.X.rel, Event.state.Y.rel);

	return true;
}

// Key has been pressed
bool _Framework::keyPressed(const OIS::KeyEvent &Event) {
	if(State)
		State->KeyEvent(Event.key, true);

	return true;
}

// Key has been released
bool _Framework::keyReleased(const OIS::KeyEvent &Event) {
	if(State)
		State->KeyEvent(Event.key, false);

	return true;
}

// Mouse has been pressed
bool _Framework::mousePressed(const OIS::MouseEvent &Event, OIS::MouseButtonID Button) {
	if(State)
		State->MouseEvent(Button, true);

	return true;
}

// Mouse has been released
bool _Framework::mouseReleased(const OIS::MouseEvent &Event, OIS::MouseButtonID Button) {
	if(State)
		State->MouseEvent(Button, false);

	return true;
}

// Delays execution of the program
void _Framework::Delay(float Time) {
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		Sleep((DWORD)(Time * 1000));
	#else
		usleep((useconds_t)(Time * 1000000));
	#endif
}
