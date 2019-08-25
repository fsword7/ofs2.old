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
}

void Scene::resize(int w, int h)
{
	gl.resize(w, h);
}

void Scene::render(const Player *player)
{
	Camera *cam = player->getCamera(0);
	vec3f_t cpos = cam->getPosition();
	quatf_t crot = cam->getRotation();

	this->player = player;
	
	if (vobj == nullptr) {
			vobj = new vPlanet(*this);
	}

	gl.start();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prm.mproj = glm::perspective(glm::radians(OFS_DEFAULT_FOV), float(gl.getWidth()) / float(gl.getHeight()), DIST_NEAR, DIST_FAR);
	prm.mview = glm::transpose(glm::toMat4(crot));
	prm.mview = glm::translate(prm.mview, cpos);

	vobj->render(prm);

	gl.finish();
}
