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

Engine::Engine(Player *_player)
: player(_player),
  realTime(0), scaleTime(1.0)
{
	pdata = "data/";
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

void Engine::update(float dt)
{
	// Update current julian time
	realTime += dt / SECONDS_PER_DAY;
	if (player != nullptr)
		player->update(dt, scaleTime);
}
