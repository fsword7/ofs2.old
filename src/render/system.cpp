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
//	if (pgm == nullptr) {
//		ShaderManager &smgr = scene.getShaderManager();
//
//		pgm = smgr.createShader("star");
//
//	    vbuf = new VertexBuffer(ctx, 1);
//	   	vbo = vbuf->createBuffer(VertexBuffer::VBO, 1);
//	}
//
//	pgm->use();
//	vbuf->bind();

//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, 120000 * sizeof(starVertex), nullptr, GL_STREAM_DRAW);
//	vertices = reinterpret_cast<starVertex *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
//	if (vertices == nullptr) {
//		cerr << "Can't render stars - aborted (error code: " << glGetError() << ")" << endl;
//		return;
//	}
//
//	glEnable(GL_PROGRAM_POINT_SIZE);
//
//	mat4f_t mvp = mat4f_t (prm.dmProj * prm.dmView * mat4d_t(1.0));
//
//	uint32_t mvpLoc = glGetUniformLocation(pgm->getID(), "mvp");
//    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
//	if (!glUnmapBuffer(GL_ARRAY_BUFFER)) {
//		cerr << "Buffer corrupted - aborted (error code: " << glGetError() << ")" << endl;
//		return;
//	}
//	vertices = nullptr;
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(starVertex), (void *)0);
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(starVertex), (void *)(3 * sizeof(float)));
//	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(starVertex), (void *)(7 * sizeof(float)));
//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
//	glEnableVertexAttribArray(2);
//
//	// Now rendering stars
//	glDrawArrays(GL_POINTS, 0, 1);
//
//	glDisableVertexAttribArray(0);
//	glDisableVertexAttribArray(1);
//	glDisableVertexAttribArray(2);
//	vbuf->unbind();
//
//	glUseProgram(0);
//	glDisable(GL_PROGRAM_POINT_SIZE);
}

void Scene::renderObjectAsPoint()
{
}

void Scene::renderObject(vObject *vobj)
{
	// setupObjectLighting
	vobj->render(prm);
}

void Scene::renderPlanetarySystem(const SystemTree *tree)
{
	int nObjects = tree->getSystemSize();

	for (int idx = 0; idx < nObjects; idx++) {
		Object *object = tree->getObject(idx);

		{
			vObject *vobj = getVisualObject(object, true);

			renderObject(vobj);
		}
	}

	// Rendering satellites orbiting around this celestial body
//	const SystemTree *subtree = body.getSystemTree();
//	if (subtree != nullptr)
//		renderPlanetarySystem(subtree);
}

void Scene::buildObjectList(const SystemTree *tree)
{
}

void Scene::buildOrbitList(const SystemTree *tree)
{
}
