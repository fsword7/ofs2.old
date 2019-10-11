/*
 * system.cpp - rendering planetary system package
 *
 *  Created on: Oct 7, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/player.h"
#include "universe/astro.h"
#include "universe/body.h"
#include "universe/star.h"
#include "universe/frame.h"
#include "universe/system.h"

#include "render/gl/shader.h"
#include "render/gl/buffer.h"
#include "render/planet.h"
#include "render/render.h"

using namespace ofs::astro;
using namespace ofs::universe;

void Scene::setupLightingObject(vector<LightSource> &sun,
	const vec3d_t objPosition, const quatd_t objRotation,
	LightState &ls)
{
	uint32_t nLights = min((uint32_t)MAX_LIGHTS, (uint32_t)sun.size());

	if (nLights == 0)
		return;
	for (int idx = 0; idx < nLights; idx++)
	{
		vec3d_t spos  = sun[idx].pos - objPosition;
		double  dist  = glm::length(spos);
		double  au    = convertKilometerToAU(dist);

		ls.lights[idx].sunPosition  = spos;
		ls.lights[idx].apparentSize = (sun[idx].radius / dist);
		ls.lights[idx].color        = sun[idx].color;
		ls.lights[idx].irradiance   = sun[idx].luminosity / (au * au);
		ls.lights[idx].directEye    = spos * (1.0 / dist);
		ls.lights[idx].directObject = ls.lights[idx].directEye * objRotation;
		ls.lights[idx].castShadows  = true;
	}

	ls.eyePosObject = -objPosition * objRotation;
	ls.eyeDirObject = (vec3d_t(0, 0, 0) - objPosition) * objRotation;
	ls.ambientColor = vec3d_t(0, 0, 0);
}

void Scene::renderPlanetarySystem(const CelestialStar *sun)
{
	const System *system = sun->getSystem();
	const SystemTree *tree = system->getSystemTree();
	int nObjects = tree->getSystemSize();

	for (int idx = 0; idx < nObjects; idx++) {
		Object *object = tree->getObject(idx);

		{
			vObject *vobj = getVisualObject(object, true);

			vobj->render(prm);
		}
	}
}



