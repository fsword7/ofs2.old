/*
 * render.cpp - OFS scene renderer package
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/player.h"
#include "render/gl/shader.h"
#include "render/gl/buffer.h"
#include "render/planet.h"
#include "render/render.h"

void Scene::init(int w, int h)
{
	gl.init(w, h);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Initialize global parameters
	TerrainManager::ginit(*this);

	initStarVertex();
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
	return 2 * tan(cam->getFOV()/2.0) / double(gl.getHeight());
}

void Scene::render(const Player *player, const Universe *universe)
{
	Camera *cam = player->getCamera(0);
	
	prm.jdTime  = player->getJulianTime();
	prm.cpos    = cam->getPosition();
	prm.crot    = cam->getRotation();
	prm.tanap   = cam->getTanAp();
	
	this->player = player;
	
	nearStars.clear();
	lightSources.clear();

	if (vobj1 == nullptr) {
		vobj1 = new vPlanet(*this, *universe->getEarth1());
		vobj2 = new vPlanet(*this, *universe->getEarth2());
	}

	// Find closest stars within desired distance
	vec3d_t obs = player->getPosition();
	universe->findNearStars(obs, 1.0, nearStars);
	setupLightSources(nearStars, obs, prm.jdTime, lightSources);

//	cout << "Closest star list: (" << nearStars.size() << " stars)" << endl;
//	for(const CelestialStar *star : nearStars) {
//		if (star->getHIPNumber() == 0)
//			cout << "HIP " << star->getHIPNumber() << " " << star->getName()
//			<< " Distance: " << glm::length(star->getPosition(0)) << endl;
//	}

	gl.start();

//	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//	prm.mproj = glm::perspective(glm::radians(OFS_DEFAULT_FOV), float(gl.getWidth()) / float(gl.getHeight()), DIST_NEAR, DIST_FAR);
//	prm.mview = glm::toMat4(prm.crot);
//	prm.mview = glm::translate(prm.mview, prm.cpos);

	prm.dmProj  = glm::perspective(glm::radians(DOFS_DEFAULT_FOV), double(gl.getWidth()) / double(gl.getHeight()), DIST_NEAR, DIST_FAR);
	prm.dmView  = glm::transpose(glm::toMat4(prm.crot));
//	prm.dmPView = prm.dmProj * prm.dmView;
//	prm.mProj = mat4f_t(prm.dmProj);
//	prm.mView = mat4f_t(prm.dmView);

	// Render visible stars
	renderStars(*universe->getStarCatalogue(), *player, faintestMagNight);

	// Render constellation lines
//	renderConstellations(*universe, *player);

	vobj2->render(prm);
	vobj1->render(prm);

	gl.finish();
}
