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

	// Initialize global parameters
	TerrainManager::ginit(*this);
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

void Scene::render(const Player *player)
{
	Camera *cam = player->getCamera(0);
	
	prm.jdTime  = player->getJulianTime();
	prm.cpos    = cam->getPosition();
	prm.crot    = cam->getRotation();
	prm.tanap   = cam->getTanAp();
	
	this->player = player;
	
	if (vobj == nullptr) {
		vobj = new vPlanet(*this);
	}

	gl.start();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prm.mproj = glm::perspective(glm::radians(OFS_DEFAULT_FOV), float(gl.getWidth()) / float(gl.getHeight()), DIST_NEAR, DIST_FAR);
	prm.mview = glm::transpose(glm::toMat4(prm.crot));
	// prm.mview = glm::translate(prm.mview, prm.cpos);

	vobj->render(prm);

	gl.finish();
}
