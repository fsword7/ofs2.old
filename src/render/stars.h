/*
 * stars.h
 *
 *  Created on: Sep 23, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "universe/handler.h"
//#include "universe/starlib.h"
#include "render/starcolors.h"
//#include "render/texture.h"

//class glStarVertex : public StarVertex
//{
//public:
//	glStarVertex(const Scene &scene, int maxStars = 1000);
//	~glStarVertex();
//
//	// Render routines
//	void startPoints() override;
//	void startSprites() override;
//	void render() override;
//	void finish() override;
//
//private:
//	glShaderPackage *pkg;
//};

class StarVertex
{
public:
	enum pointType {
		useNotUsed = 0,
		usePoints,
		useSprites
	};

	struct starVertex
	{
		vec3f_t		posStar;
		Color		color;
		float		size;
	};

	StarVertex(Scene &scene);
	~StarVertex();

	// Render routines
	void start();
	void render();
	void finish();

	void addStar(const vec3d_t &pos, const Color &color, double size);
//	void setTexture(Texture *image);

protected:
	Scene &scene;
	Context &ctx;
	renderParameter &prm;

	pointType type;
	int  nStars;
	bool flagStarted;

	ShaderProgram *pgm = nullptr;
    VertexBuffer *vbuf = nullptr;

//	Texture *txImage = nullptr;

    starVertex *vertices = nullptr;
    GLuint vbo;
};

class StarRenderer : public ofsHandler
{
public:
	StarRenderer() = default;
	~StarRenderer()
	{
		if (starBuffer != nullptr)
			delete starBuffer;
	}

	void process(const CelestialStar& star, double dist, double appMag) const;

public:
	vec3d_t obsPos = { 0, 0, 0 }; // Observer's camera position

	// Star buffer for rendering
	StarVertex *starBuffer = nullptr;

	Scene   *scene = nullptr;     // Scene handler
	Context *context = nullptr;
	vec3d_t  cpos;    // Current camera/player position
	double   pxSize;  // Pixel size
	double   baseSize;

	float    faintestMag = 0.0f;
	float    faintestMagNight = 0.0f;
	float    saturationMag = 0.0f;

	StarColors *starColors = nullptr;

};
