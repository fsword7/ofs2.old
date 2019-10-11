/*
 * engine.cpp
 *
 *  Created on: Oct 9, 2018
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "main/date.h"
#include "engine/engine.h"
#include "engine/player.h"
#include "universe/astro.h"
#include "universe/body.h"
#include "render/render.h"

using namespace ofs::astro;
using namespace ofs::universe;

Engine::Engine()
: realTime(0), scaleTime(1.0)
{
	// Initialize database parameters
	dataFolder   = "data";
	systemFolder = dataFolder + "/systems";

	universe.setEngine(this);
	universe.init();
}

Engine::~Engine()
{
}

void Engine::init(uint32_t height, uint32_t width)
{
	scene = new Scene();
	scene->init(height, width, universe);
}

void Engine::start()
{
	// Start julian clock as of now
	realDate.reset();
	realTime = realDate;

	cout << "Start date: " << realDate.getString() << endl;
	cout << " Real Time: " << Date(realTime).getString() << endl;

	// Initialize real julian time
	scaleTime = 1.0;
	player.start(realTime);

	// To removed later

	CelestialBody *earth = universe.getEarth();
	player.go(*earth);
	player.follow(*earth);
}

void Engine::update(double dt)
{
	// Update current julian time
	realTime += dt / SECONDS_PER_DAY;
	player.update(dt, scaleTime);
}
