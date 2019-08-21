/*
 * engine.h
 *
 *  Created on: Oct 9, 2018
 *      Author: Tim Stark
 */

#pragma once

#include "main/date.h"

//class Universe;
class Player;

class Engine
{
public:
	Engine(Player *player);
	~Engine();

	inline Date *getRealTime() { return &realDate; }

//	inline string getDataFolder() const { return pdata; }

	void start();
	void update(float dt);

private:
//	Universe *universe;
	Player   *player;

	Date 	realDate;	// current Julian clock
	float	realTime;	// System time in Julian clock
	float	scaleTime;	// Time-travel scale time [Julian date]

	string	pdata; // data folders for file access
};
