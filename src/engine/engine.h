/*
 * engine.h
 *
 *  Created on: Oct 9, 2018
 *      Author: Tim Stark
 */

#pragma once

#include "main/date.h"
#include "engine/player.h"
#include "universe/universe.h"

using namespace ofs::universe;

class Engine
{
public:
	Engine();
	~Engine();

	inline Universe *getUniverse() { return &universe; }
	inline Player *getPlayer() { return &player; }
	inline Date *getRealTime() { return &realDate; }

//	inline string getDataFolder() const { return pdata; }

	void start();
	void update(float dt);

private:
	Player   player;
	Universe universe;

	Date 	realDate;	// current Julian clock
	float	realTime;	// System time in Julian clock
	float	scaleTime;	// Time-travel scale time [Julian date]

	string	pdata; // data folders for file access
};
