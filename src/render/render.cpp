/*
 * render.cpp - OFS scene renderer package
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/player.h"
#include "universe/body.h"
#include "universe/star.h"

#include "render/gl/shader.h"
#include "render/gl/buffer.h"
#include "render/planet.h"
#include "render/render.h"

using namespace ofs::universe;

void Scene::init(int w, int h, Universe &universe)
{
	gl.init(w, h);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Initialize global parameters
	TerrainManager::ginit(*this);

	initStarVertex();
	initConstellations(universe);
}

void Scene::cleanup()
{
	// Cleanup global parameters
	TerrainManager::gexit();
}

void Scene::resize(int w, int h)
{
	gl.resize(w, h);
}

double Scene::calculatePixelSize(Camera *cam) const
{
	return (2.0 * tan(cam->getFOV()/2.0)) / double(gl.getHeight());
}

vObject *Scene::addVisualObject(Object *object)
{
//	vObjectList *pvo = new vObjectList();
	vObject *vobj;

	cout << "Adding visual object '" << object->getName() << "'..." << endl;

	vobj = vObject::create(object, this);

	vobjList.push_back(vobj);

//	pvo = new vObjectList();
//	pvo->vobj = vobj;

	// Add visual object to list
//	pvo->prev = vobjFirst;
//	pvo->next = nullptr;
//	if (vobjLast != nullptr)
//		vobjLast->next = pvo;
//	else
//		vobjFirst = pvo;
//	vobjLast = pvo;

	return vobj;
}

vObject *Scene::getVisualObject(Object *object, bool createFlag)
{
	if (object == nullptr)
		return nullptr;
	for (int idx = 0; idx < vobjList.size(); idx++) {
		if (vobjList[idx]->getObject() == object)
			return vobjList[idx];
	}
	if (createFlag == true)
		return addVisualObject(object);
	return nullptr;
}

void Scene::render(const Player *player, const Universe *universe)
{
	Camera *cam = player->getCamera(0);
	
	prm.now   = player->getJulianTime();
	prm.cpos  = cam->getPosition();
	prm.crot  = cam->getRotation();
	prm.tanap = cam->getTanAp();
	
	this->player = player;
	this->universe = universe;
	
	nearStars.clear();
	lightSources.clear();

	// Find closest stars within desired distance
	vec3d_t obs = player->getPosition();
	universe->findNearStars(obs, 1.0, nearStars);
	setupPrimaryLightSources(nearStars, obs, prm.now, lightSources);

//	cout << "Closest star list: (" << nearStars.size() << " stars)" << endl;
//	for(const CelestialStar *star : nearStars) {
//		if (star->getHIPNumber() == 0)
//			cout << "HIP " << star->getHIPNumber() << " " << star->getName()
//			<< " Distance: " << glm::length(star->getPosition(0)) << endl;
//	}

	gl.start();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prm.dmProj  = glm::perspective(cam->getFOV(), double(gl.getWidth()) / double(gl.getHeight()), DIST_NEAR, DIST_FAR);
	prm.dmOrtho = glm::ortho(0, gl.getWidth(), gl.getHeight(), 0);
	prm.dmView  = glm::transpose(glm::toMat4(prm.crot));
//	prm.dmPView = prm.dmProj * prm.dmView;
//	prm.mProj = mat4f_t(prm.dmProj);
//	prm.mView = mat4f_t(prm.dmView);

	// Render constellation lines
	renderConstellations(*universe, *player);

	// Render visible stars
	renderStars(*universe->getStarCatalogue(), *player, faintestMagNight);

	// Render planetary systems
	for (int idx = 0; idx < nearStars.size(); idx++) {
		const CelestialStar *sun = nearStars[idx];

		if (!sun->hasSystem())
			continue;

		const System *system = sun->getSystem();
		const SystemTree *tree = system->getSystemTree();

		renderPlanetarySystem(tree);
	}

	gl.finish();
}
