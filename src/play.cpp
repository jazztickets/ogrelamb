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

#include "play.h"
#include "framework.h"
#include "log.h"
#include "config.h"
#include "objectmanager.h"
#include "physics.h"
#include "camera.h"
#include "object.h"
#include "character.h"
#include "vehicle.h"
#include "actions.h"
#include "templates.h"
#include "template.h"
#include <OgreTerrain.h>
#include <OgreSceneManager.h>
#include <OgreVector3.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletDynamics/Vehicle/btRaycastVehicle.h>

_PlayState PlayState;
static Ogre::Light *Light = NULL;

// Initializes the state
void _PlayState::Init() {
	Player = Car = Character = NULL;
	Time = 0.0f;
	UpdateCount = 0;

	// Set up mapping
	Actions.AddKeyMap(OIS::KC_E, _Actions::Forward);
	Actions.AddKeyMap(OIS::KC_D, _Actions::Back);
	Actions.AddKeyMap(OIS::KC_S, _Actions::Left);
	Actions.AddKeyMap(OIS::KC_F, _Actions::Right);
	Actions.AddKeyMap(OIS::KC_SPACE, _Actions::Jump);
	Actions.AddKeyMap(OIS::KC_A, _Actions::Sprint);
	Actions.AddKeyMap(OIS::KC_LSHIFT, _Actions::Ascend);
	Actions.AddKeyMap(OIS::KC_SPACE, _Actions::Descend);
	Actions.AddKeyMap(OIS::KC_E, _Actions::Vehicle_Accel);
	Actions.AddKeyMap(OIS::KC_D, _Actions::Vehicle_Reverse);
	Actions.AddKeyMap(OIS::KC_S, _Actions::Vehicle_SteerLeft);
	Actions.AddKeyMap(OIS::KC_F, _Actions::Vehicle_SteerRight);
	Actions.AddKeyMap(OIS::KC_SPACE, _Actions::Vehicle_Brake);
	Actions.AddKeyMap(OIS::KC_A, _Actions::Vehicle_Flip);
	Actions.AddMouseMap(OIS::MB_Left, _Actions::Vehicle_Boost);
	Actions.AddMouseMap(OIS::MB_Left, _Actions::Fast);
	Actions.AddMouseMap(OIS::MB_Right, _Actions::Faster);

	Physics.Init();
	ObjectManager.Init();
	Camera.SetPosition(btVector3(0, 2, 5));
	Camera.Type = _Camera::THIRD_PERSON;
	//Camera.Type = _Camera::FREEMOVE;
	Camera.Distance = 10.0f;
	Camera.Sensitivity[0] = Config.MouseX;
	Camera.Sensitivity[1] = Config.MouseY;
	Camera.HandleMouse(0, 0);

	Ogre::Vector3 LightDirection(1, -1, 0);
	LightDirection.normalise();
	Light = Game.Scene->createLight();
	Light->setType(Ogre::Light::LT_DIRECTIONAL);
	Light->setDirection(LightDirection);
	Light->setDiffuseColour(Ogre::ColourValue::White);
	Light->setSpecularColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));

	Game.Scene->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));

	Game.TerrainGlobalOptions->setCompositeMapAmbient(Game.Scene->getAmbientLight());
	Game.TerrainGlobalOptions->setCompositeMapDiffuse(Light->getDiffuseColour());
	Game.TerrainGlobalOptions->setLightMapDirection(LightDirection);
	Game.TerrainGlobalOptions->setMaxPixelError(8);

	//ObjectManager.CreateObject(_Spawn(Templates["room"], "room"));
	//ObjectManager.CreateObject(_Spawn(Templates["terrain"], "terrain"));
	ObjectManager.CreateObject(_Spawn(Templates["terrain_ogre"], "terrain_ogre"));

	Character = ObjectManager.CreateObject(_Spawn(Templates["character"], "character", btVector3(0, 10, 5)));
	Car = ObjectManager.CreateObject(_Spawn(Templates["car"], "player", btVector3(0, 10, 0), btQuaternion(btRadians(180), 0, 0)));
	//Ball = ObjectManager.CreateObject(Templates.Templates["sphere"], btVector3(0, 0.5, -4));

	if(0) {
		int BaseCount = 5;
		float Y = 0.5;
		while(BaseCount >= 0) {
			for(int i = 0; i < BaseCount; i++) {
				ObjectManager.CreateObject(_Spawn(Templates["crate"], "box", btVector3(-BaseCount / 2.0f + i, Y, -3)));
			}
			BaseCount--;
			Y++;
		}
	}

	Player = Character;
	Camera.FollowObject = Player;
	Physics.Enabled = true;
}

// Shuts the state down
void _PlayState::Close() {
	Game.Scene->destroyAllLights();
	ObjectManager.Close();
	Physics.Close();
}

// Updates the current state
void _PlayState::Update(float FrameTime) {
	static float JumpTimer = 0.0f;

	float CameraSpeed = 0.1f;
	btVector3 Move(0, 0, 0);
	if(Actions.GetState(_Actions::Forward))
		Move[2] += -1;
	if(Actions.GetState(_Actions::Back))
		Move[2] += 1;
	if(Actions.GetState(_Actions::Left))
		Move[0] += -1;
	if(Actions.GetState(_Actions::Right))
		Move[0] += 1;
	if(Actions.GetState(_Actions::Ascend))
		Move[1] += -1;
	if(Actions.GetState(_Actions::Descend))
		Move[1] += 1;
	if(Actions.GetState(_Actions::Fast))
		CameraSpeed *= 10.0f;
	if(Actions.GetState(_Actions::Faster))
		CameraSpeed *= 10.0f;

	Camera.HandleMove(Move, CameraSpeed);

	if(Player && Camera.Type != _Camera::FREEMOVE) {

		if(Player->Character) {
			float Speed = 5.00f * FrameTime;

			if(Actions.GetState(_Actions::Sprint)) {
				Speed *= 2;
			}
			Move.setY(0);
			if(!Move.isZero())
				Move.normalize();

			btTransform CameraDirection;
			CameraDirection.setIdentity();
			CameraDirection.setRotation(btQuaternion(-Camera.Yaw, 0, 0));

			Move = CameraDirection * Move;
			Move *= Speed;
			Player->Character->Controller->setWalkDirection(Move);

			if(Actions.GetState(_Actions::Jump) && Player->Character->Controller->canJump()) {
				Player->Character->Controller->jump();
			}

		}

		if(Player->Vehicle) {
			btRaycastVehicle *Vehicle = Player->Vehicle->RaycastVehicle;
			//printf("%f\n", Vehicle->getCurrentSpeedKmHour());
			if(Actions.GetState(_Actions::Vehicle_Brake)) {
				Vehicle->setBrake(2, 2);
				Vehicle->setBrake(2, 3);
				Vehicle->applyEngineForce(0, 2);
				Vehicle->applyEngineForce(0, 3);
			}
			else if(Actions.GetState(_Actions::Vehicle_Accel)) {
				float Speed = 5;
				if(Actions.GetState(_Actions::Vehicle_Boost))
					Speed *= 2;
				Vehicle->applyEngineForce(Speed, 2);
				Vehicle->applyEngineForce(Speed, 3);
			}
			else if(Actions.GetState(_Actions::Vehicle_Reverse)) {
				Vehicle->applyEngineForce(-3, 2);
				Vehicle->applyEngineForce(-3, 3);
			}
			else {
				Vehicle->applyEngineForce(0, 2);
				Vehicle->applyEngineForce(0, 3);
				Vehicle->setBrake(0, 2);
				Vehicle->setBrake(0, 3);
			}

			if(Actions.GetState(_Actions::Vehicle_SteerLeft)) {
				Player->Vehicle->SetSteer(0.01f);
			}
			else if(Actions.GetState(_Actions::Vehicle_SteerRight)) {
				Player->Vehicle->SetSteer(-0.01f);
			}
			else {
				Player->Vehicle->ReleaseSteer(1.5f);
			}

			if(JumpTimer > 0.0f)
				JumpTimer -= FrameTime;
			else if(Actions.GetState(_Actions::Vehicle_Flip)) {
				btTransform Trans = Player->Vehicle->RaycastVehicle->getRigidBody()->getWorldTransform();
				Trans.setOrigin(btVector3(0, 0, 0));
				btVector3 Push(0, 0, 700);
				Push = Trans * Push;
				//Player->Vehicle->RaycastVehicle->getRigidBody()->applyForce(btVector3(0, 400, 0), Push);
				Player->Vehicle->RaycastVehicle->getRigidBody()->applyTorque(Push);
				JumpTimer = 2.0f;
			}
		}
	}

	ObjectManager.Update(FrameTime);
	Physics.Update(FrameTime);
	Camera.Update(FrameTime);

	Time += FrameTime;
	UpdateCount++;
}

// Interpolate object transforms
void _PlayState::UpdateRender(float TimeStepRemainder, float TimeStep) {
	float BlendFactor = TimeStepRemainder / TimeStep;
	Physics.World->setTimeStepRemainder(TimeStepRemainder);
	Physics.World->synchronizeMotionStates();

	//btVector3 Position = Player->GraphicsTransform.getOrigin();
	//Light->setPosition(Position.x(), Position.y() + 2, Position.z());
	Camera.UpdateRender(BlendFactor);
}

// Draws the current state
void _PlayState::Draw() {

}

// Key presses
void _PlayState::KeyEvent(int Key, bool Pressed) {

	if(Pressed) {
		switch(Key) {
			case OIS::KC_ESCAPE:
				//if(!TerrainGroup->isDerivedDataUpdateInProgress())
					Game.Done = true;
			break;
			case OIS::KC_F1:
				Camera.Sensitivity[1] *= -1;
			break;
			case OIS::KC_F2:
				Game.Camera->setPolygonMode((Ogre::PolygonMode)((int)Ogre::PM_SOLID - (int)Game.Camera->getPolygonMode() + (int)Ogre::PM_WIREFRAME));
			break;
			case OIS::KC_F3:
				if(Camera.Type == _Camera::THIRD_PERSON)
					Camera.Type = _Camera::FREEMOVE;
				else if(Camera.Type == _Camera::FREEMOVE && Camera.FollowObject)
					Camera.Type = _Camera::THIRD_PERSON;
			break;
			case OIS::KC_W:
				Camera.Distance = 10.0f - Camera.Distance;
				Camera.HandleMouse(0, 0);
			break;
			case OIS::KC_1:
				ObjectManager.CreateObject(_Spawn(Templates["crate"], "", btVector3(0, 7, -5.25)));
			break;
			case OIS::KC_2:
				ObjectManager.CreateObject(_Spawn(Templates["sphere"], "", btVector3(0, 4, -4)));
			break;
			case OIS::KC_T:
				if(Player == Character)
					Player = Car;
				else
					Player = Character;

				Camera.FollowObject = Player;
			break;
			case OIS::KC_X:
				Game.ChangeState(&PlayState);
			break;
		}
	}

	Actions.KeyEvent(Key, Pressed);
}

// Mouse motion
void _PlayState::MouseMoved(int UpdateX, int UpdateY) {
	Camera.HandleMouse(UpdateX, UpdateY);
}

// Mouse presses
void _PlayState::MouseEvent(int Button, bool Pressed) {
	Actions.MouseEvent(Button, Pressed);
}
