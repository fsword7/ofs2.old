/*
 * core.cpp - OFS Core application
 *
 *  Created on: Oct 2, 2018
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "main/date.h"
#include "engine/engine.h"
#include "engine/player.h"
//#include "universe/universe.h"
#include "render/render.h"
#include "main/coreapp.h"

using namespace ofs;
//using namespace ofsu;

CoreApp::CoreApp()
: width(OFS_DEFAULT_WIDTH),
  height(OFS_DEFAULT_HEIGHT)
{
	// Initialize state keys
	for (int key = 0; key < 256; key++)
		stateKey[key] = false;

	// Initialize velocity control
	keyRotationAccel = toRadian(OFS_DEFAULT_FOV);
	keyTravelAccel   = 2.0;
	keyTravelBrake   = 10.0;
}

CoreApp::~CoreApp()
{
	if (scene != nullptr)
		delete scene;
//	if (engine != nullptr)
//		delete engine;
}

void CoreApp::initRenderer()
{
	// Initialize OpenGL-based Renderer
	scene = new Scene();
	scene->init(width, height);
}

void CoreApp::initEngine()
{

	engine = new Engine();
	player = engine->getPlayer();
}

void CoreApp::pressKey(keyCode code, bool down)
{
	stateKey[code] = down;
}

void CoreApp::start()
{
	if (engine != nullptr)
		engine->start();
}

void CoreApp::tick()
{
	Date *jdate = engine->getRealTime();

	float   dt;
	vec3f_t av, tv;

	dt = jdate->update();
	av = player->getAngularVelocity();
	tv = player->getTravelVelocity();

	// Keyboard rotation and travel control
	// X-axis rotation control
	if (stateKey[keyPad8] || stateKey[keyUp])
		av += vec3f_t(dt * keyRotationAccel, 0, 0);
	if (stateKey[keyPad2] || stateKey[keyDown])
		av += vec3f_t(dt * -keyRotationAccel, 0, 0);

	// Y-axis rotation control
	if (stateKey[keyPad4] || stateKey[keyLeft])
		av += vec3f_t(0, dt * keyRotationAccel, 0);
	if (stateKey[keyPad6] || stateKey[keyRight])
		av += vec3f_t(0, dt * -keyRotationAccel, 0);

	// Z-axis rotation control
	if (stateKey[keyPad7])
		av += vec3f_t(0, 0, dt * -keyRotationAccel);
	if (stateKey[keyPad9])
		av += vec3f_t(0, 0, dt * keyRotationAccel);

	// Travel velocity control
	if (stateKey[keyPad3])
		tv.z += dt /* * 1000.0 */;
	if (stateKey[keyPad1])
		tv.z -= dt /* * 1000.0 */;

	// Braking velocity control
	if (stateKey[keyPad5])
	{
		av *= exp(-dt * keyRotationAccel);
		tv *= exp(-dt * keyTravelAccel);
	}

	player->setAngularVelocity(av);
	player->setTravelVelocity(tv);
	engine->update(dt);
}

void CoreApp::render()
{
//	if (scene != nullptr && player != nullptr && universe != nullptr)
//		scene->render(*player, *universe);
	if (scene != nullptr)
		scene->render(player);
}

void CoreApp::resize(int w, int h)
{
	width = w;
	height = h;
	if (scene != nullptr)
		scene->resize(w, h);
}
