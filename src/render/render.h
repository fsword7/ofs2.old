/*
 * render.h
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/gl/context.h"

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	void init(int width, int height);
	void resize(int width, int height);

	void render();

private:
	Context gl;
};
