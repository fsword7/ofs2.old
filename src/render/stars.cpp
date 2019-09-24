/*
 * stars.cpp - Rendering stars package
 *
 *  Created on: Sep 23, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/player.h"
#include "universe/astro.h"
#include "render/gl/shader.h"
#include "render/gl/buffer.h"
#include "render/render.h"
#include "render/stars.h"

using namespace ofs::astro;
using namespace ofs::universe;

StarVertex::StarVertex(const Scene &scene, int maxStars)
: scene(scene),
  type(useNotUsed),
  maxStars(maxStars),
  nStars(0),
  flagStarted(false)
{
	buffer.resize(maxStars);
}

StarVertex::~StarVertex()
{
	buffer.clear();
}

//void StarVertex::setTexture(Texture *image)
//{
//	txImage = image;
//}

void StarVertex::addStar(const vec3d_t &pos, const Color &color, double size)
{
	if (nStars == maxStars)
		render();

	buffer[nStars].posStar = pos;
	buffer[nStars].size = size;
	buffer[nStars].color[0] = color.getRed();
	buffer[nStars].color[1] = color.getGreen();
	buffer[nStars].color[2] = color.getBlue();
	buffer[nStars].color[3] = color.getAlpha();

	nStars++;
}

// ****************************************************************

void StarRenderer::process(const CelestialStar& star, double dist, double appMag) const
{
	vec3d_t spos, rpos;
	double  srad;
	double  rdist;
	double  objSize;
	double  alpha, ptSize;
	Color   color;

	// Calculate relative position between star and camera positions.
	spos  = star.getPosition(0) * KM_PER_PC;
	rpos  = spos - cpos;
	rdist = glm::length(rpos);

	// Calculate apparent size of star in view field
	srad    = star.getRadius();
	objSize = (srad / dist) / pxSize;

	alpha  = clamp(faintestMag - appMag);
	ptSize = objSize * 5.0;
	color  = starColors->lookup(star.getTemperature());
	color.setAlpha(alpha);

	// Finally, now display star
//	cout << "@@@ Adding a star..." << endl;
	starBuffer->addStar(spos, color, ptSize);
}

// ************************************************************************

//double Scene::computeApparentMagnitude(const CelestialBody &obj, vec3d_t vpos)
//{
//	double appMag = numeric_limits<double>::infinity();
//
//	for (int idx = 0; idx < lightSources.size(); idx++)
//		appMag = min(appMag, obj.getApparentMagnitude(lightSources[idx].luminosity,
//			vpos - lightSources[idx].pos, vpos));
//
//	return appMag;
//}

void Scene::setupLightSources(const vector<const CelestialStar *> &nearStars,
		const vec3d_t &obs, double now, vector<LightSource> &ls)
{
	for (int idx = 0; idx < nearStars.size(); idx++) {
		const CelestialStar *star = nearStars[idx];
		LightSource ls;

		ls.pos        = star->getPosition(now) - (obs / KM_PER_PC);
		ls.luminosity = star->getLuminosity();
		ls.radius     = star->getRadius();
		ls.color      = starColors.lookup(star->getTemperature());

		lightSources.push_back(ls);
	}
}

void Scene::initVisibleStars()
{
	std::string fname = "data/stars/bbr_color_D58.txt";
	starColors.load(fname);
}

void Scene::renderStars(StarCatalogue &starlib, const Player& player, double faintestMag)
{
	StarRenderer starHandler;
	vec3d_t obs = player.getPosition();
	quatd_t rot = player.getRotation();
	Camera *cam = player.getCamera(0);
	double  fov = cam->getFOV();
	double  aspect = cam->getAspect();

	starHandler.scene = this;
	starHandler.cpos = cam->getPosition();
//	starHandler.pxSize = calculatePixelSize();
	starHandler.faintestMag = faintestMag;
	starHandler.starColors = &starColors;

//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
////	std::cout << "### Starting star renderer..." << std::endl;
//	starHandler.starVertex->start();
////	stardb.findVisibleStars(starHandler, player, faintestMag);
//	stardb.findVisibleStars(starHandler, obs, rot, fov, aspect, faintestMag);
//	starHandler.starVertex->finish();
//	glDisable(GL_BLEND);
}
