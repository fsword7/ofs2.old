/*
 * render.h
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "universe/universe.h"
#include "render/context.h"
#include "render/gl/buffer.h"
#include "render/gl/shader.h"

#define DIST_NEAR	0.000001f // 0.001f
#define DIST_FAR	10.0f     // 1.0e9f

class Player;
class vPlanet;

struct renderParameter
{
	double jdTime;    // Current julian time

	glm::mat4 mproj;  // Projection matrix
	glm::mat4 mview;  // View matrix
	glm::mat4 model;  // Model matrix (current object)
	glm::mat4 mvp;

	// Camera parameters (global)
	vec3f_t cpos;
	quatf_t crot;
	float   tanap;

	// Object parameters (reference frame)
	struct {
		uint32_t maxLOD;
		uint32_t biasLOD;
		vec3f_t  opos;
		mat4f_t  orot;
		float    orad;

		vec3f_t  cpos;
		mat4f_t  crot;
		vec3f_t  cdir;
		float    cdist;

		float    viewap;
		float    tanap;
	} obj;

};

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	inline ShaderManager &getShaderManager() { return smgr; }
	inline const Context *getContext() const { return &gl; }

	void init(int width, int height);
	void cleanup();
	
	void resize(int width, int height);

	void render(const Player *player, const Universe *universe);

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
