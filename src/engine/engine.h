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

class Scene;

class Engine
{
public:
	Engine();
	~Engine();

	inline Universe *getUniverse() { return &universe; }
	inline Player *getPlayer() { return &player; }
	inline Scene *getScene() { return scene; }
	inline Date *getRealTime() { return &realDate; }

//	inline string getDataFolder() const { return pdata; }

	void init(uint32_t height, uint32_t width);
	void start();
	void update(double dt);

private:
	Scene   *scene = nullptr;
	Player   player;
	Universe universe;

	Date 	realDate;	// current Julian clock
	double	realTime;	// System time in Julian clock
	double	scaleTime;	// Time-travel scale time [Julian date]

	string	pdata; // data folders for file access
};
