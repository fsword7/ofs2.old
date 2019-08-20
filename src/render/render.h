/*
 * render.h
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/gl/context.h"
#include "render/gl/shader.h"

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

	ShaderManager smgr;
	ShaderProgram *demo = nullptr;

	uint32_t vao, vbo, ebo;
	uint32_t myColor;
	uint32_t texture1, texture2;

};
