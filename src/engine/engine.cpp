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
#include "render/overlay.h"

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

	overlay = new Overlay(scene);

	Context *gl = scene->getContext();
	titleFont = TextureFont::load(*gl, "data/fonts/OpenSans-Bold.ttf", 20);
	textFont =  TextureFont::load(*gl, "data/fonts/OpenSans-Regular.ttf", 12);

}

void Engine::resize(uint32_t width, uint32_t height)
{
	scene->resize(width, height);
	overlay->reset();
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


//	Object *planet = universe.findPath("Sol/Mercury");
//	Object *planet = universe.findPath("Sol/Venus");
	Object *planet = universe.findPath("Sol/Earth");
//	Object *planet = universe.findPath("Sol/Mars");
//	Object *planet = universe.findPath("Sol/Jupiter");
//	Object *planet = universe.findPath("Sol/Saturn");
//	Object *planet = universe.findPath("Sol/Uranus");
//	Object *planet = universe.findPath("Sol/Neptune");

	if (planet != nullptr) {
		player.go(*planet, planet->getRadius() * 6.0);
//		player.follow(*planet, Player::fwGeosync);
		player.follow(*planet, Player::fwHelioSync);
	} else
		cout << "Planet is not found in universe!!" << endl;
}

void Engine::update(double dt)
{
	// Update current julian time
	realTime += dt / SECONDS_PER_DAY;
	player.update(dt, scaleTime);
}

void Engine::render()
{
	if (scene == nullptr)
		return;

	scene->render(&player, &universe);
	renderOverlay();
}
