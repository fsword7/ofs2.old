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

#define DIST_NEAR	0.0001f
#define DIST_FAR	1.0e6f
#define DDIST_NEAR	0.0001
#define DDIST_FAR	1.0e6

class Player;
class vPlanet;

struct renderParameter
{
	double jdTime;    // Current julian time

	mat4f_t mProj;  // Projection matrix
	mat4f_t mView;  // View matrix
	mat4f_t mWorld;  // Model matrix (current object)
	mat4f_t mPView;
	mat4f_t mvp;

	mat4d_t dmProj;
	mat4d_t dmView;
	mat4d_t dmWorld;
	mat4d_t dtWorld;
	mat4d_t dmPView;
	mat4d_t dmvp;

	// Camera parameters (global)
	vec3d_t cpos;
	quatd_t crot;
	float   tanap;

	// Object parameters (reference frame)
	struct {
		uint32_t maxLOD;
		uint32_t biasLOD;
		vec3d_t  opos;
		mat4d_t  orot;
		double   orad;

		vec3d_t  cpos;
		mat4d_t  crot;
		vec3d_t  cdir;
		double   cdist;

		double   viewap;
		double   tanap;
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
