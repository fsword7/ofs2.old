/*
 * render.h
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "universe/universe.h"
#include "universe/frame.h"
#include "universe/system.h"

#include "render/context.h"
#include "render/starcolors.h"

#define DIST_NEAR	0.0001
//#define DIST_FAR	1.0e15
#define DIST_FAR 1.0e10

class Player;
class Camera;
class vObject;
class vPlanet;
class StarRenderer;
class TextureFont;

struct renderParameter
{
	double  now;    // Current julian time

	mat4f_t mProj;  // Projection matrix
	mat4f_t mView;  // View matrix
	mat4f_t mWorld;  // Model matrix (current object)
	mat4f_t mPView;
	mat4f_t mvp;

	mat4d_t dmProj;
	mat4d_t dmOrtho;
	mat4d_t dmView;
	mat4d_t dmModel;
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
		Color    color;
		uint32_t maxLOD;
		uint32_t biasLOD;
		vec3d_t  opos;
		quatd_t  oqrot;
		mat4d_t  orot;
		double   orad;

		vec3d_t  cpos;
		quatd_t  cqrot;
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
	vec3d_t spos;
	double  luminosity;
	double  radius;
	Color   color;
};

// Reflected object
struct SecondaryLight
{
	const Object *object;
	vec3d_t viewPosition;
	double  radius;
	double  reflected;
};

struct DirectLight
{
	vec3d_t sunPosition;
	vec3d_t directObject;
	vec3d_t directEye;
	Color   color;
	double  irradiance;
	double  apparentSize;
	bool	castShadows;
};

struct LightState
{
	LightState()
	: nLights(0), eyeDirObject(0, 0, -1), eyePosObject(0, 0, -1)
	{}

	vec3d_t     eyeDirObject;
	vec3d_t     eyePosObject;
	vec3d_t     ambientColor;
	uint32_t    nLights;
	DirectLight lights[MAX_LIGHTS];
};

struct VertexLine
{
	vec3f_t lpos;
	Color   color;
};

struct Annotation
{
	string	labelText;
	Color	color;
	vec3f_t	position;
	double	size;
};

struct ObjectListEntry
{
	Object *object;		// Object

	vec3d_t opos;		// Object position
	vec3d_t spos;		// Sun position

	double  dist;		// Distance
	double	discSize;	// Disc size (in pixels)
	double  appMag;		// Apparent magnitude

	double	zNear;		// Near Z clipping
	double	zCenter;	// Center Z clipping
	double	zFar;		// Far Z clipping
};

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	inline ShaderManager &getShaderManager() { return gl.getShaderManager(); }
	inline const Context *getContext() const { return &gl; }
	inline Context *getContext() { return &gl; }
	inline renderParameter *getParameter() { return &prm; }
	inline double getAspect() const { return gl.getAspect(); }

	inline int getWidth() const { return gl.getWidth(); }
	inline int getHeight() const { return gl.getHeight(); }

	inline const Engine *getEngine() const { return universe->getEngine(); }

	void init(int width, int height, Universe &universe);
	void cleanup();
	
	void resize(int width, int height);

	double computePixelSize(Camera *cam) const;

	vObject *addVisualObject(Object *object);
	vObject *getVisualObject(Object *object, bool createFlag);

	void render(const Player *player, const Universe *universe);

protected:
	void initStarVertex();
	void initConstellations(const Universe &universe);

	void buildObjectList(const SystemTree *tree);
	void buildOrbitList(const SystemTree *tree);

	void renderStars(const StarCatalogue &starlib, const Player& player, double faintestMag);
	void renderConstellations(const Universe &universe, const Player &player);

	void renderPoint(const vec3d_t &pos, const Color &color, double size);
	void renderObjectAsPoint();
	void renderCelestialBody(vObject *vobj);
	void renderPlanetarySystem(const SystemTree *tree, vec3d_t frameCenter,
		const Player *player, vec3d_t apos, double now);

	void renderOverlay();
	void renderHeadupPanel();

	// Light source function calls
	void setupPrimaryLightSources(const vector<const CelestialStar *> &nearStars,
			const vec3d_t &obs, double now, vector<LightSource> &ls);
	void setupSecondaryLightSources();
	void setupObjectLighting(vector<LightSource> &suns, const vec3d_t opos,
			const quatd_t orot, LightState &ls);

	vec3d_t computeAstrocentricPosition(const CelestialStar *sun,
			vec3d_t upos, double now);

private:
	Context gl;

	const Player   *player = nullptr;
	const Universe *universe = nullptr;

	vector<const CelestialStar *> nearStars;
	vector<vObject *> vobjList;
	vector<LightSource> lightSources;
	vector<ObjectListEntry> objectList;

	StarColors *starColors = nullptr;
	StarRenderer  *starRenderer = nullptr;

	TextureFont *titleFont = nullptr;
	TextureFont *textFont = nullptr;

	renderParameter prm;

	ShaderProgram *pgmStar = nullptr;
	VertexBuffer *vbufStar = nullptr;

	ShaderProgram *pgmAsterism = nullptr;
	VertexBuffer *vbufAsterism = nullptr;
	uint32_t asterismLines = 0;

	double pixelSize = 0.0;

	float faintestMag      = 6.0f;
	float faintestMagNight = faintestMag;
	float saturationMag    = 1.0f;
};
