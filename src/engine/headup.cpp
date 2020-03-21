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
#include "universe/body.h"
#include "render/overlay.h"

// Rendering overlay/headup panel
void Engine::renderOverlay()
{
	double curTime = player.getCurrentTime();

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

void Engine::displayPlanetocentric(double lon, double lat, double alt)
{
	char lonHemi, latHemi;

	lonHemi = lon < 0.0 ? 'W' : lon > 0.0 ? 'E' : ' ';
	latHemi = lat < 0.0 ? 'S' : lat > 0.0 ? 'N' : ' ';

	double dlon = abs(glm::degrees(lon));
	double dlat = abs(glm::degrees(lat));

	string locObject = fmt::sprintf("Location: %.6f%c %.6f%c", dlon, lonHemi, dlat, latHemi);
	overlay->print(locObject);
}

void Engine::displayPlanetInfo()
{
	const Object *center = player.getCenter();
	double curTime = player.getCurrentTime();
	vec3d_t view = center->getPosition(curTime) - player.getPosition();

	overlay->setFont(titleFont);
	overlay->print(center->getName());

	overlay->setFont(textFont);
	string distObject = fmt::sprintf("Distance: %f", glm::length(view));
	string radObject = fmt::sprintf("Radius: %f", center->getRadius());
	overlay->print(distObject);
	overlay->print(radObject);

	const vec3d_t opos = center->getPosition(curTime);
	const vec3d_t hpos = dynamic_cast<const CelestialBody *>(center)->getHeliocentric(curTime);
	const vec3d_t pc = dynamic_cast<const CelestialBody *>(center)->getPlanetocentricFromEcliptic(view, curTime);
	displayPlanetocentric(pc.x, pc.y, 0);

	string hObject = fmt::sprintf("Heliocentric: (%lf,%lf)", glm::degrees(hpos.x), glm::degrees(hpos.y));
	string posObject = fmt::sprintf("Astrocentric: (%lf,%lf,%lf)", opos.x, opos.y, opos.z);
	string pcObject = fmt::sprintf("Position: (%.6f,%.6f,%.6f)", view.x, view.y, view.z);

	overlay->print(hObject);
	overlay->print(posObject);
	overlay->print(pcObject);

	lastView = view;
}
