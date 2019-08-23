/*
 * render.h
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/gl/context.h"
#include "render/gl/buffer.h"
#include "render/gl/shader.h"

class Player;
class vPlanet;

struct renderParameter
{
	glm::mat4 mproj;  // Projection matrix
	glm::mat4 mview;  // View matrix
	glm::mat4 model;  // Model matrix (current object)
};

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	void init(int width, int height);
	void resize(int width, int height);

	void render(const Player *player);

private:
	Context gl;

	const Player *player;

	renderParameter prm;

	ShaderManager smgr;
	ShaderProgram *demo = nullptr;

	VertexBuffer *glBuffer;

	uint32_t vao, vbo, ebo;
	uint32_t myColor;
	uint32_t texture1, texture2;

	vPlanet *vobj = nullptr;
};
