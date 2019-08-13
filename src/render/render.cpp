/*
 * render.cpp - OFS scene renderer package
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/render.h"

void Scene::init(int w, int h)
{
	gl.init(w, h);
}

void Scene::resize(int w, int h)
{
	gl.resize(w, h);
}

void Scene::render()
{

}
