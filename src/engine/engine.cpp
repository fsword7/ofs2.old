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

using namespace ofs::astro;
using namespace ofs::universe;

Engine::Engine()
: realTime(0), scaleTime(1.0)
{
//	player.setPosition(vec3f_t(0.0, 0.0, KM_PER_AU));
	player.setPosition(vec3f_t(0.0, 0.0, 0.0));

	pdata = "data/";

	universe.init();
}

Engine::~Engine()
{
}

void Engine::start()
{
	// Start julian clock as of now
	realDate.reset();
	realTime = realDate;
}

void Engine::update(double dt)
{
	// Update current julian time
	realTime += dt / SECONDS_PER_DAY;
	player.update(dt, scaleTime);
}
