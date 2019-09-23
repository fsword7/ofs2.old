/*
 * stars.cpp - Rendering stars package
 *
 *  Created on: Sep 23, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/player.h"
#include "render/gl/shader.h"
#include "render/gl/buffer.h"
#include "render/render.h"


//StarVertex::StarVertex(const Scene &scene, int maxStars)
//: scene(scene),
//  type(useNotUsed),
//  maxStars(maxStars),
//  nStars(0),
//  flagStarted(false)
//{
//	buffer.resize(maxStars);
//}
//
//StarVertex::~StarVertex()
//{
//	buffer.clear();
//}
//
//void StarVertex::setTexture(Texture *image)
//{
//	txImage = image;
//}
//
//void StarVertex::addStar(const vec3d_t &pos, const Color &color, double size)
//{
//	if (nStars == maxStars)
//		render();
//
//	buffer[nStars].posStar = pos;
//	buffer[nStars].size = size;
////	buffer[nStars].color = color;
//
//	buffer[nStars].color[0] = color.rgba[0];
//	buffer[nStars].color[1] = color.rgba[1];
//	buffer[nStars].color[2] = color.rgba[2];
//	buffer[nStars].color[3] = color.rgba[3];
//
//	nStars++;
//}

// ****************************************************************

//void StarRenderer::process(const CelestialStar& star, double dist, double appMag) const
//{
//	vec3d_t spos, rpos;
//	double  srad;
//	double  rdist;
//	double  objSize;
//	double  alpha, ptSize;
//	Color   color;
//
//	// Calculate relative position between star and camera positions.
//	spos  = star.getPosition(0) * KM_PER_PC;
//	rpos  = spos - cpos;
//	rdist = glm::length(rpos);
//
//	// Calculate apparent size of star in view field
//	srad    = star.getRadius();
//	objSize = (srad / dist) / pxSize;
//
//	alpha  = clamp(faintestMag - appMag);
//	ptSize = objSize * 5.0;
//	color  = starColors->lookup(star.getTemperature());
//	color.setAlpha(alpha);
//
//	// Finally, now display star
////	std::cout << "@@@ Adding a star..." << std::endl;
//	starBuffer->addStar(spos, color, ptSize);
//}

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
//
//void Scene::setupLightSources(const std::vector<const CelestialStar *> &nearStars,
//		const vec3d_t &obs, double now, std::vector<LightSource> &ls)
//{
//	for (int idx = 0; idx < nearStars.size(); idx++) {
//		const CelestialStar *star = nearStars[idx];
//		LightSource ls;
//
//		ls.pos        = star->getPosition(now) - obs;
//		ls.luminosity = star->getLuminosity();
//		ls.radius     = star->getRadius();
//		ls.color      = starColors->lookup(star->getTemperature());
//
//		lightSources.push_back(ls);
//	}
//}

void Scene::initVisibleStars()
{
//	starColors = new StellarColors();
//
//	std::string fname = "../data/bbr_color_D58.txt";
//	starColors->load(fname);
}

void Scene::renderStars(StarCatalogue &starlib, const Player& player, double faintestMag)
{
//	StarHandler starHandler;
//	vec3d_t obs = player.getPosition() / KM_PER_PC;
//	quatd_t rot = player.getOrientation();
//	double  fov = cam->getFOVRad();
//	double  aspect = cam->getAspect();
//
//	starHandler.scn = this;
//	starHandler.cpos = cam->getPosition();
//	starHandler.pxSize = calculatePixelSize();
//	starHandler.faintestMag = faintestMag;
//	starHandler.starColors = starColors;
//
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
////	std::cout << "### Starting star renderer..." << std::endl;
//	starHandler.starVertex->start();
////	stardb.findVisibleStars(starHandler, player, faintestMag);
//	stardb.findVisibleStars(starHandler, obs, rot, fov, aspect, faintestMag);
//	starHandler.starVertex->finish();
//	glDisable(GL_BLEND);
}
