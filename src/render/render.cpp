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
	resize(w, h);
}

void Scene::resize(int w, int h)
{
	width  = w;
	height = h;
	aspect = double(width)/double(height);

	glViewport(0, 0, w, h);

	cout << "Width: " << w << " Height: " << h << " Aspect: " << aspect << endl;
}

void Scene::render()
{

}
