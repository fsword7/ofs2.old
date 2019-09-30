/*
 * stars.cpp - Rendering stars package
 *
 *  Created on: Sep 23, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/player.h"
#include "universe/astro.h"
#include "universe/universe.h"
#include "render/gl/shader.h"
#include "render/gl/buffer.h"
#include "render/render.h"
#include "render/stars.h"

using namespace ofs::astro;
using namespace ofs::universe;


StarVertex::StarVertex(Scene &scene, int maxStars)
: scene(scene),
  ctx(*scene.getContext()),
  prm(*scene.getParameter()),
  type(useNotUsed),
  maxStars(maxStars),
  nStars(0), cStars(0),
  flagStarted(false)
{
	buffer = new starVertex[maxStars];
}

StarVertex::~StarVertex()
{
	finish();
	if (buffer != nullptr)
		delete []buffer;
}

//void StarVertex::setTexture(Texture *image)
//{
//	txImage = image;
//}

void StarVertex::startPoints()
{
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_COLOR_ARRAY);
//	glDisableClientState(GL_NORMAL_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
//	glDisable(GL_TEXTURE_2D);
//
//	uint32_t stride = sizeof(starVertex);
//	glVertexPointer(3, GL_DOUBLE, stride, &buffer[0].posStar);
//	glColorPointer(4, GL_UNSIGNED_SHORT, stride, &buffer[0].color);
//	glPointSize(1.0);
//
//	nStars = 0;
//	cStars = 0;
//	type = usePoints;
//	flagStarted = true;
}

void StarVertex::startSprites()
{
	if (pgm == nullptr) {
		ShaderManager &smgr = scene.getShaderManager();

		pgm = smgr.createShader("star");

	    vbuf = new VertexBuffer(ctx, 1);
	   	vbuf->createBuffer(VertexBuffer::VBO, 1);
	}

	pgm->use();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

//	cout << "starVertex size: " << sizeof(starVertex) << endl;
//	cout << "  vec3f_t size:  " << sizeof(vec3f_t) << endl;
//	cout << "  Color size:    " << sizeof(Color) << endl;

//	glDisable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
//	glEnable(GL_TEXTURE_2D);
	glPointSize(1.0);

	mat4f_t mvp = mat4f_t (prm.dmProj * prm.dmView * mat4d_t(1.0));

	uint32_t mvpLoc = glGetUniformLocation(pgm->getID(), "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	nStars = 0;
	cStars = 0;
	type = useSprites;
	flagStarted = true;
}

void StarVertex::render()
{
	if (nStars == 0)
		return;

    vbuf->bind();
	vbuf->assign(VertexBuffer::VBO, buffer, nStars*sizeof(starVertex));

	// Now rendering stars
//	if (txImage != nullptr)
//		txImage->bind();
	glDrawArrays(GL_POINTS, 0, nStars);
	cStars += nStars;
	nStars  = 0;
}

void StarVertex::finish()
{

	render();

	flagStarted = false;

//	cout << "Total " << cStars << " rendered stars." << endl;
	cStars = 0;

//	glDisableClientState(GL_COLOR_ARRAY);
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	switch (type) {
	case useSprites:
//		glUseProgram(0);
		glDisable(GL_PROGRAM_POINT_SIZE);
		glDisable(GL_TEXTURE_2D);
//		glEnable(GL_DEPTH_TEST);
		break;
	case usePoints:
	default:
		break;
	}
	type = useNotUsed;
}

void StarVertex::addStar(const vec3d_t &pos, const Color &color, double size)
{
	if (nStars == maxStars)
		render();

	buffer[nStars].posStar = pos;
	buffer[nStars].color = color;
	buffer[nStars].size = size;

	nStars++;
}

// ****************************************************************

void StarRenderer::process(const CelestialStar& star, double dist, double appMag) const
{
	vec3d_t spos, rpos;
	double  srad;
	double  rdist;
	double  objSize;
	double  discSize;
	double  discScale;
	double  alpha, ptSize;
	Color   color;

	// Calculate relative position between star and camera positions.
	spos  = star.getPosition(0) * KM_PER_PC;
	rpos  = spos - cpos;
	rdist = glm::length(rpos);

	// Calculate apparent size of star in view field
	srad    = star.getRadius();
//	objSize = (srad / (dist * KM_PER_PC)) / pxSize;
	objSize = srad / (dist * pxSize * KM_PER_PC);

	alpha  = faintestMag - appMag;
	discSize = baseSize;
	if (alpha > 1.0) {
		discScale = min(pow(2.0, 0.3 * (saturationMag - appMag)), 100.0);
		discSize *= discScale;
		alpha = 1.0;
	} else if (alpha < 0.0)
		alpha = 0.0;

	color  = starColors->lookup(star.getTemperature());
	color.setAlpha(alpha);

//	if (star.getHIPNumber() == 0) {
//		cout << "Yes, Sun is here." << endl;
//		cout << "Star size: " << discSize << " Position: " << rpos.x << "," << rpos.y << "," << rpos.z << endl;
//		cout << "Star color: " << color.getRed() << "," << color.getGreen() << "," << color.getBlue() << "," << color.getAlpha() << endl;
//		discSize = 20.0;
////		rpos = -rpos;
////		starBuffer->addStar(rpos, color, discSize);
//	}

	// Finally, now display star
//	cout << "@@@ Adding a star..." << endl;
	starBuffer->addStar(rpos, color, discSize);
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
		ls.color      = starColors->lookup(star->getTemperature());

		lightSources.push_back(ls);
	}
}

void Scene::initStarVertex()
{
	StarVertex *starBuffer;
	std::string fname = "data/stars/bbr_color_D58.txt";

	starColors = new StarColors();
	starColors->load(fname);

	// Create Gaussian star/glare texture mapping
//	starTexture = createStarTexture(8);
//	glareTexture = createGlareTexture(8);

	starBuffer = new StarVertex(*this, 2048);
//	starBuffer->setTexture(starTexture);

	starRenderer = new StarRenderer();
	starRenderer->scene = this;
	starRenderer->context = &gl;
	starRenderer->starBuffer = starBuffer;
	starRenderer->starColors = starColors;
}

void Scene::renderStars(const StarCatalogue &starlib, const Player &player,
	double faintest)
{
	vec3d_t obs = player.getPosition();
	quatd_t rot = player.getRotation();
	Camera *cam = player.getCamera(0);
	double  fov = cam->getFOV();
	double  aspect = gl.getAspect();

	starRenderer->cpos = cam->getPosition();
	starRenderer->pxSize = calculatePixelSize(cam);
	starRenderer->baseSize = 5.0; // default base star size
	starRenderer->faintestMag = faintestMag;
	starRenderer->faintestMagNight = faintest;
	starRenderer->saturationMag = saturationMag;
//	starRenderer->starColors = starColors;
//	starRenderer->starBuffer->startPoints();
	starRenderer->starBuffer->startSprites();

//	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
////	std::cout << "### Starting star renderer..." << std::endl;
////	stardb.findVisibleStars(starHandler, player, faintestMag);
	starlib.findVisibleStars(*starRenderer, obs, rot, fov, aspect, faintest);
	starRenderer->starBuffer->finish();
	glDisable(GL_BLEND);
//	glEnable(GL_DEPTH_TEST);
}

void Scene::renderConstellations(const Universe &universe, const Player &player)
{
	const Constellations &constellations = universe.getAsterism();
	const StarCatalogue &starlib = *universe.getStarCatalogue();

	Camera *cam = player.getCamera(0);
	vec3d_t cpos = cam->getPosition();
	vec3d_t buffer[2];

	const vector<Asterism *> &asterisms = constellations.getAsterisms();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, &buffer[0]);
	glColor4f(0.2, 0.2, 0.2, 1.0);

	for (int idx = 0; idx < asterisms.size(); idx++) {
		Asterism *aster = asterisms[idx];
		for (int sidx = 0; sidx < aster->hip.size(); sidx += 2) {
			CelestialStar *star1 = starlib.getHIPStar(aster->hip[sidx]);
			CelestialStar *star2 = starlib.getHIPStar(aster->hip[sidx+1]);

			if (star1 == nullptr)
				std::cout << "HIP " << aster->hip[sidx] << " Missing" << std::endl;
			if (star2 == nullptr)
				std::cout << "HIP " << aster->hip[sidx+1] << " Missing" << std::endl;
			if (star1 == nullptr || star2 == nullptr)
				continue;

			buffer[0] = star1->getPosition(0) * KM_PER_PC;
			buffer[1] = star2->getPosition(0) * KM_PER_PC;
//			std::cout << "HIP: " << aster->hip[sidx]
//					  << " Name: " << star->name(0) << std::endl;
			glDrawArrays(GL_LINES, 0, 2);
		}
//		std::cout << std::endl;
	}

	glDisableClientState(GL_VERTEX_ARRAY);
}


//void glScene::createStarTexture(uint8_t *data, int mip, int scale)
//{
//	int size = 1 << mip;
//	float fwhm = (float)scale * 0.3f;
//	float sigma = fwhm / 2.3548f;
//	float isig2 = 1.0f / (2.0f * sigma * sigma);
//	float s = 1.0f / (sigma * (float)sqrt(2.0 * 3.14159));
//
////	printf("FWHM: %f\n", fwhm);
////	printf("Sigma: %f\n", sigma);
////	printf("isig2: %f\n", isig2);
////	printf("s: %f\n", s);
//
//	for (int i = 0; i < size; i++) {
//		float y = (float)i - size/2.0;
//		for (int j = 0; j < size; j++) {
//			float x = (float)j - size/2.0;
//			float r2 = x * x + y * y;
//			float f = s * (float)exp(-r2 * isig2) * (float)scale;
//
//			data[i * size + j] = std::min(f, 1.0f) * 255.99;
////			printf("%03d ", (unsigned int)(std::min(f, 1.0f) * 255.99));
//		}
////		printf("\n");
//	}
////	printf("\n");
//}
//
//void glScene::createGlareTexture(uint8_t *data, int mip, int scale)
//{
//	int size = 1 << mip;
//	float s = 25.0f / (float)scale;
//	float base = 0.66f;
//
//	for (int i = 0; i < size; i++) {
//		float y = (float)i - size/2.0;
//		for (int j = 0; j < size; j++) {
//			float x = (float)j - size/2.0;
//
//			float r = sqrt(x * x + y * y);
//			float f = pow(base, r * s);
//
//			data[i * size + j] = std::min(f, 1.0f) * 255.99;
////			printf("%03d ", (unsigned int)(std::min(f, 1.0f) * 255.99));
//		}
////		printf("\n");
//	}
////	printf("\n");
//}
//
//Texture *glScene::createStarTexture(int mip)
//{
//	int size = 1 << mip;
//
//	Texture *txImage = new glTexture(size, size, mip+1);
//	txImage->setMipMode(Texture::FixedMipMaps);
//	txImage->setFormat(GL_LUMINANCE);
//
//	for (int lod = 0; lod < mip; lod++) {
//		int scale = pow(2.0, mip - lod);
//		createStarTexture(txImage->getMipData(lod), mip - lod, scale);
//	}
//
//	return txImage;
//}
//
//Texture *glScene::createGlareTexture(int mip)
//{
//	int size = 1 << mip;
//
//	Texture *txImage = new glTexture(size, size, mip+1);
//	txImage->setMipMode(Texture::FixedMipMaps);
//	txImage->setFormat(GL_LUMINANCE);
//
//	for (int lod = 0; lod < mip; lod++) {
//		int scale = pow(2.0, mip - lod);
//		createGlareTexture(txImage->getMipData(lod), mip - lod, scale);
//	}
//
//	return txImage;
//}
