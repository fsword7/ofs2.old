/*
 * system.cpp - rendering planetary system package
 *
 *  Created on: Oct 7, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/player.h"
#include "universe/body.h"
#include "universe/star.h"
#include "universe/frame.h"
#include "universe/system.h"

#include "render/gl/shader.h"
#include "render/gl/buffer.h"
#include "render/planet.h"
#include "render/render.h"

using namespace ofs::universe;

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



