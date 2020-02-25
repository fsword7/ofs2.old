/*
 * headup.cpp
 *
 *  Created on: Feb 25, 2020
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/engine.h"
#include "engine/object.h"
#include "engine/headup.h"
#include "render/overlay.h"

// Rendering overlay/headup panel
void Engine::renderOverlay()
{

	Color color(0.7, 0.7, 1.0, 1.0);

	overlay->home();
	overlay->moveBy(15.0, 15.0);

	overlay->setColor(color);

	displayPlanetInfo();

//	overlay->setFont(titleFont);
//	overlay->print("Earth");
//
//	overlay->setFont(textFont);
//	overlay->print("text 1");
//	overlay->print("text 2");
//	overlay->print("text 3");

//	titleFont->render("abcdefghijklmnopqrstuvwxyz", 10.0, 20.0, Color(1.0, 1.0, 0.0, 0.0));
//	titleFont->render("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 10.0, 60.0, Color(1.0, 1.0, 0.0, 0.0));
}

void Engine::displayPlanetInfo()
{
	const Object *center = player.getCenter();

	overlay->setFont(titleFont);
	overlay->print(center->getName());

	overlay->setFont(textFont);
	string radObject = fmt::sprintf("Radius: %f", center->getRadius());
	overlay->print(radObject);
}
