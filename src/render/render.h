/*
 * render.h
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "universe/universe.h"
#include "render/context.h"
#include "render/starcolors.h"
#include "render/gl/buffer.h"
#include "render/gl/shader.h"

#define DIST_NEAR	0.0001
//#define DIST_FAR	1.0e15
#define DIST_FAR 1.0e10

class Player;
class Camera;
class vPlanet;
class StarRenderer;

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
	double  aspect;

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

#define MAX_LIGHTS  8

struct LightSource
{
	vec3d_t pos;
	double  luminosity;
	double  radius;
	Color   color;
};

struct DirectLight
{
};

struct LightState
{
};

struct VertexLine
{
	vec3f_t lpos;
	Color   color;
};

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	inline ShaderManager &getShaderManager() { return smgr; }
	inline const Context *getContext() const { return &gl; }
	inline Context *getContext() { return &gl; }
	inline renderParameter *getParameter() { return &prm; }
	inline double getAspect() const { return gl.getAspect(); }

	void init(int width, int height, Universe &universe);
	void cleanup();
	
	void resize(int width, int height);

	double calculatePixelSize(Camera *cam) const;

	void render(const Player *player, const Universe *universe);

protected:
	void initStarVertex();
	void initConstellations(const Universe &universe);

	void renderStars(const StarCatalogue &starlib, const Player& player, double faintestMag);
	void renderConstellations(const Universe &universe, const Player &player);
	void setupLightSources(const vector<const CelestialStar *> &nearStars,
			const vec3d_t &obs, double now, vector<LightSource> &ls);

private:
	Context gl;

	const Player *player;

	vector<const CelestialStar *> nearStars;
//	vector<vObject *> vobjList;
	vector<LightSource> lightSources;

	StarColors *starColors = nullptr;
	StarRenderer  *starRenderer = nullptr;

	renderParameter prm;

	ShaderManager smgr;

//	ShaderProgram *demo = nullptr;
//	VertexBuffer *glBuffer;

	ShaderProgram *pgmAsterism = nullptr;
	VertexBuffer *vbufAsterism = nullptr;
	uint32_t asterismLines = 0;

//	uint32_t vao, vbo, ebo;
//	uint32_t myColor;
//	uint32_t texture1, texture2;

	float faintestMag      = 6.0f;
	float faintestMagNight = faintestMag;
	float saturationMag    = 1.0f;

	vPlanet *vobj1 = nullptr;
	vPlanet *vobj2 = nullptr;
};
