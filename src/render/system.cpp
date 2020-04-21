/*
 * system.cpp - rendering planetary system package
 *
 *  Created on: Oct 7, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/player.h"
#include "universe/astro.h"
#include "universe/body.h"
#include "universe/star.h"

#include "render/planet.h"
#include "render/render.h"
#include "render/gl/font.h"

using namespace ofs::astro;
using namespace ofs::universe;

struct LightIrradiancePredicate
{
	LightIrradiancePredicate() {};

	bool operator ()(const DirectLight& l0, const DirectLight& l1) const
	{
		return l0.irradiance > l1.irradiance;
	}
};

void Scene::setupObjectLighting(vector<LightSource> &sun,
	const vec3d_t objPosition, const quatd_t objRotation,
	LightState &ls)
{
	uint32_t nLights = min((uint32_t)MAX_LIGHTS, (uint32_t)sun.size());

	if (nLights == 0)
		return;
	for (int idx = 0; idx < nLights; idx++)
	{
		vec3d_t spos  = sun[idx].spos - objPosition;
		double  dist  = glm::length(spos);
		double  au    = convertKilometerToAU(dist);

		ls.lights[idx].sunPosition  = spos;
		ls.lights[idx].apparentSize = (sun[idx].radius / dist);
		ls.lights[idx].color        = sun[idx].color;
		ls.lights[idx].irradiance   = sun[idx].luminosity / (au * au);
		ls.lights[idx].directEye    = spos * (1.0 / dist);
		ls.lights[idx].directObject = ls.lights[idx].directEye * objRotation;
		ls.lights[idx].castShadows  = true;
	}

	// for multi-star systems
	if (nLights > 1) {
		// Calculating total irradiance from multi-star systems
		double totalIrradiance = 0.0;
		for (int idx = 0; idx < nLights; idx++)
			totalIrradiance += ls.lights[idx].irradiance;

		// Determines first brightest light sources
		sort(ls.lights, ls.lights + nLights, LightIrradiancePredicate());

//		double minVisibleFraction   = 1.0 / 10000.0;
//		double minDisplayableValue  = 1.0 / 255.0;
//		double minVisibleIrradiance = minVisibleFraction * totalIrradiance;
//		double gamma = log(minDisplayableValue) / log(minVisibleFraction);
//
//		ls.nLights = 0;
//		for (int idx = 0; idx < nLights && ls.lights[idx].irradiance > minVisibleIrradiance; idx++) {
//			ls.lights[idx].irradiance =
//					pow(ls.lights[idx].irradiance / totalIrradiance, gamma);
////			ls.lights[idx].directObject = ls.lights[idx].directEye * m;
//			ls.nLights++;
//		}
	}

//	double eyeFromCenterDistance = ls.eyePosObject.length();
//	if (eyeFromCenterDistance > 100.0 && isNormalized) {
//		double s = 100.0 / eyeFromCenterDistance;
//		ls.eyePosObject = ls.eyePosObject * s;
//	}

	ls.eyePosObject = -objPosition * objRotation;
	ls.eyeDirObject = (vec3d_t(0, 0, 0) - objPosition) * objRotation;
	ls.ambientColor = vec3d_t(0, 0, 0);
}

void Scene::renderPoint(const vec3d_t &opos, const Color &color, double size)
{
	struct starVertex
	{
		vec3f_t		posStar;
		Color		color;
		float		size;
	} *vertices;

	pgmStar->use();
	vbufStar->bind();

	glBindBuffer(GL_ARRAY_BUFFER, vbufStar->getVBO());
	glBufferData(GL_ARRAY_BUFFER, sizeof(starVertex), nullptr, GL_STREAM_DRAW);
	vertices = reinterpret_cast<starVertex *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	if (vertices == nullptr) {
		cerr << "Can't render stars - aborted (error code: " << glGetError() << ")" << endl;
		return;
	}

	vertices[0].posStar = opos;
	vertices[0].color = color;
	vertices[0].size = size;

	glEnable(GL_PROGRAM_POINT_SIZE);

	mat4f_t mvp = mat4f_t (prm.dmProj * prm.dmView * mat4d_t(1.0));

	uint32_t mvpLoc = glGetUniformLocation(pgmStar->getID(), "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	if (!glUnmapBuffer(GL_ARRAY_BUFFER)) {
		cerr << "Buffer corrupted - aborted (error code: " << glGetError() << ")" << endl;
		return;
	}
	vertices = nullptr;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(starVertex), (void *)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(starVertex), (void *)(3 * sizeof(float)));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(starVertex), (void *)(7 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Now rendering stars
	glDrawArrays(GL_POINTS, 0, 1);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	vbufStar->unbind();

	glUseProgram(0);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void Scene::renderObjectAsPoint(ObjectListEntry &ole)
{
	const CelestialBody *body = dynamic_cast<const CelestialBody *>(ole.object);

	renderPoint(ole.opos, body->getColor(), ole.objSize);
}

void Scene::renderCelestialBody(ObjectListEntry &ole)
{
	const CelestialBody *body = dynamic_cast<const CelestialBody *>(ole.object);
	double bodySize = 5.0;

	vObject *vobj = getVisualObject(ole.object, true);

	if (bodySize > 1.0 && body->hasSurface()) {
		// setupObjectLighting
		vobj->render(prm);
	} else
		renderObjectAsPoint(ole);
}

void Scene::renderPlanetarySystem(const SystemTree *tree, const Player *player,
	vec3d_t apos, vec3d_t vpnorm, vec3d_t origin, double now)
{
	int nObjects = tree->getSystemSize();

	for (int idx = 0; idx < nObjects; idx++) {
		CelestialBody *body = dynamic_cast<CelestialBody *>(tree->getObject(idx));

		{
			Frame *frame = body->getOrbitFrame();
//			Orbit *orbit = body->getOrbit();

			// Determine sun, body and player position in local reference frame
			vec3d_t opos = body->getPosition(now);
			vec3d_t spos = origin + glm::conjugate(frame->getOrientation(now)) * opos;
			vec3d_t vpos = spos - apos;

			double vdist   = glm::length(vpos);
			double vdnorm  = glm::dot(vpnorm, vpos);
			double objSize = body->getRadius() / (vdist * pixelSize);

			double appMag = 100.0;
			for (int idx = 0; idx < lightSources.size(); idx++) {
				vec3d_t sun  = vpos - lightSources[idx].spos;
				double  lum  = lightSources[idx].luminosity;
				double  mag  = body->getApparentMagnitude(sun, lum, vpos);
				appMag = min(appMag, mag);
			}

//			cout << fmt::sprintf("Object:          %s\n", body->getName());
//			cout << fmt::sprintf("Sun Position:    (%lf,%lf,%lf)\n", spos.x, spos.y, spos.z);
//			cout << fmt::sprintf("Camera Position: (%lf,%lf,%lf)\n", apos.x, apos.y, apos.z);
//			cout << fmt::sprintf("Object Position: (%lf,%lf,%lf)\n", opos.x, opos.y, opos.z);
//			cout << fmt::sprintf("View Position:   (%lf,%lf,%lf)\n", vpos.x, vpos.y, vpos.z);
//			cout << fmt::sprintf("View Distance:   %lf (%lf)\n", vdist, vdnorm);
//			cout << fmt::sprintf("Object size:     %lf (%lf)\n", objSize, pixelSize);
//			cout << fmt::sprintf("View Plane Norm: (%lf,%lf,%lf)\n\n",
//					vpnorm.x, vpnorm.y, vpnorm.z);
//			cout << fmt::sprintf("App Magnitude:   %lf\n", appMag);

			if (objSize > 1)
			{
				ObjectListEntry ole;

				ole.object  = body;
				ole.spos    = spos;
				ole.opos    = vpos;
				ole.vdist   = vdist;
				ole.objSize = objSize;
				ole.appMag  = appMag;
//				ole.zCenter = glm::dot(vpos, vzmat);

				renderCelestialBody(ole);
			}

			// Rendering satellites orbiting around this celestial body
//			const SystemTree *subtree = object->getOwnSystem();
//			if (subtree != nullptr)
//				renderPlanetarySystem(subtree, player, apos, vpnorm, spos, now);
		}
	}
}

void Scene::buildObjectList(const SystemTree *tree)
{
}

void Scene::buildOrbitList(const SystemTree *tree)
{
}
