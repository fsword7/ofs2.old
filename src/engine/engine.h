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
class Overlay;
class TextureFont;

class Engine
{
public:
	Engine();
	~Engine();

	inline Universe *getUniverse() { return &universe; }
	inline Player *getPlayer()     { return &player; }
	inline Scene *getScene()       { return scene; }
	inline Date *getRealTime()     { return &realDate; }
	inline double getTimeWarp()    { return scaleTime; }

	inline string getDataFolder() const   { return dataFolder; }
	inline string getSystemFolder() const { return systemFolder; }

	inline void setTimeWarp(double scale) { scaleTime = scale; }

	void init(uint32_t height, uint32_t width);
	void resize(uint32_t width, uint32_t height);
	void start();
	void update(double dt);
	void render();

	// Rendering overlay/headup panel
	void renderOverlay();
	void displayPlanetocentric(double lon, double lat, double alt);
	void displayPlanetInfo();

private:
	Player   player;
	Universe universe;

	Date 	realDate;	// current Julian clock
	double	realTime;	// System time in Julian clock
	double	scaleTime;	// Time-travel scale time [Julian date]

	// Database parameters
	string	dataFolder;   // data folder for file access
	string  systemFolder; // system folder

	// Renderer
	Scene   *scene = nullptr;

	// Overlay/HUD panel
	TextureFont *titleFont = nullptr;
	TextureFont *textFont = nullptr;
	Overlay     *overlay = nullptr;
	vec3d_t      lastView;
};
