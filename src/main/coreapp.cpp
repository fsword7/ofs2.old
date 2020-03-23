/*
 * core.cpp - OFS Core application
 *
 *  Created on: Oct 2, 2018
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "main/date.h"
#include "main/view.h"
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
	for (int key = 0; key < 512; key++)
		stateKey[key] = false;

	// Initialize velocity control
	keyRotationAccel = toRadian(OFS_DEFAULT_FOV);
	keyRotationBrake = toRadian(OFS_DEFAULT_FOV) * 3.0;
	keyTravelAccel   = 2.0;
	keyTravelBrake   = 5.0;

	lastX = -1;
	lastY = -1;
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
//	scene = new Scene();
//	scene->init(width, height);
}

void CoreApp::initEngine()
{
	engine = new Engine();
	engine->init(width, height);

	player = engine->getPlayer();
	universe = engine->getUniverse();
	scene = engine->getScene();

	View *view = new View(View::viewMainWindow, player, scene, 0.0f, 0.0f, 1.0f, 1.0f);
	views.push_back(view);
}

View *CoreApp::pickView(float x, float y)
{
	if (views.size() > 0)
		return views[0];
	return nullptr;
}

void CoreApp::keyPress(keyCode code, int modifiers, bool down)
{
	stateKey[code] = down;
}

void CoreApp::keyEntered(char32_t ch, int modifiers)
{
//	cout << fmt::sprintf("Key pressed: %c\n", (char)ch) << flush;
	switch (ch) {
	case L'k':
		engine->setTimeWarp(engine->getTimeWarp() * 2.0);
		break;
	case L'l':
		engine->setTimeWarp(engine->getTimeWarp() / 2.0);
		break;
	}
}

void CoreApp::mouseMove(float x, float y, int state)
{
	float dx = x - lastX;
	float dy = y - lastY;

	// Rotate camera around
	if (checkAllFlags(state, mouseLeftButton|mouseRightButton)) {
		if (checkAnyFlags(state, mouseControlKey)) {
			double dzoom = dy / height;
			player->dolly(dzoom * 5);
		} else {
			Camera *cam = player->getCamera(0);
			double fov  = cam->getFOV();

			double coarseness = player->computeCoarseness(1.5);

			quatd_t rot = xrot(dy / height * coarseness) * yrot(dx / width * coarseness);
			player->orbit(rot);
		}
	} else if (state & mouseLeftButton) {
		Camera *cam = player->getCamera(0);
		double fov  = cam->getFOV();

		double coarseness = glm::degrees(fov) / 30.0;

		quatd_t rot = xrot(dy / height * coarseness) * yrot(dx / width * coarseness);
		player->rotate(rot);
	}

	// Save current mouse motion for next event.
	lastX = x;
	lastY = y;
}

void CoreApp::mousePressButtonDown(float x, float y, int state)
{
}

void CoreApp::mousePressButtonUp(float x, float y, int state)
{
	View *view = nullptr;
	float vx = 0.0f, vy = 0.0f;

	if (state & mouseLeftButton) {
		view = pickView(x, y);
		if (view != nullptr)
			view->map(x/float(width), y/float(height), vx, vy);

		vec3d_t ray = player->getRay(vx, vy);

	}
}

void CoreApp::mouseDialWheel(float motion)
{
}


void CoreApp::start()
{
	if (engine != nullptr)
		engine->start();
}

void CoreApp::tick()
{
	Date *jdate = engine->getRealTime();

	double  dt;
	vec3d_t av, tv;

	dt = jdate->update();
	av = player->getAngularVelocity();
	tv = player->getTravelVelocity();

	// Keyboard rotation and travel control
	// X-axis rotation control
	if (stateKey[keyPad8] || stateKey[keyUp])
		av += vec3d_t(dt * keyRotationAccel, 0, 0);
	if (stateKey[keyPad2] || stateKey[keyDown])
		av += vec3d_t(dt * -keyRotationAccel, 0, 0);

	// Y-axis rotation control
	if (stateKey[keyPad4] || stateKey[keyLeft])
		av += vec3d_t(0, dt * keyRotationAccel, 0);
	if (stateKey[keyPad6] || stateKey[keyRight])
		av += vec3d_t(0, dt * -keyRotationAccel, 0);

	// Z-axis rotation control
	if (stateKey[keyPad7])
		av += vec3d_t(0, 0, dt * -keyRotationAccel);
	if (stateKey[keyPad9])
		av += vec3d_t(0, 0, dt * keyRotationAccel);

	// Travel velocity control
	if (stateKey[keyPad3])
		tv.z += dt * 3000.0;
	if (stateKey[keyPad1])
		tv.z -= dt * 3000.0;

	// Braking velocity control
	if (stateKey[keyPad5])
	{
		av *= exp(-dt * keyRotationBrake);
		tv *= exp(-dt * keyTravelBrake);
	}

	player->setAngularVelocity(av);
	player->setTravelVelocity(tv);
	engine->update(dt);
}

void CoreApp::render()
{
//	if (scene != nullptr && player != nullptr && universe != nullptr)
//		scene->render(*player, *universe);
//	if (scene != nullptr)
//		scene->render(player, universe);
	engine->render();
}

void CoreApp::resize(int w, int h)
{
	width = w;
	height = h;
	engine->resize(w, h);
//	if (scene != nullptr)
//		scene->resize(w, h);
}
