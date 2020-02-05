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
#include "render/gl/context.h"
#include "render/gl/shader.h"
#include "render/gl/buffer.h"
#include "render/render.h"
#include "render/stars.h"

using namespace ofs::astro;
using namespace ofs::universe;


StarVertex::StarVertex(Scene &scene)
: scene(scene),
  ctx(*scene.getContext()),
  prm(*scene.getParameter()),
  type(useNotUsed),
  nStars(0), vbo(0),
  flagStarted(false)
{
}

StarVertex::~StarVertex()
{
}

//void StarVertex::setTexture(Texture *image)
//{
//	txImage = image;
//}

void StarVertex::start()
{
	if (pgm == nullptr) {
		ShaderManager &smgr = scene.getShaderManager();

		pgm = smgr.createShader("star");

	    vbuf = new VertexBuffer(ctx, 1);
	   	vbo = vbuf->createBuffer(VertexBuffer::VBO, 1);
	}

	pgm->use();
	vbuf->bind();

//	cout << "starVertex size: " << sizeof(starVertex) << endl;
//	cout << "  vec3f_t size:  " << sizeof(vec3f_t) << endl;
//	cout << "  Color size:    " << sizeof(Color) << endl;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 120000 * sizeof(starVertex), nullptr, GL_STREAM_DRAW);
	vertices = reinterpret_cast<starVertex *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	if (vertices == nullptr) {
		cerr << "Can't render stars - aborted (error code: " << glGetError() << ")" << endl;
		return;
	}

	glEnable(GL_PROGRAM_POINT_SIZE);

	mat4f_t mvp = mat4f_t (prm.dmProj * prm.dmView * mat4d_t(1.0));

	uint32_t mvpLoc = glGetUniformLocation(pgm->getID(), "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	nStars = 0;
	type = useSprites;
	flagStarted = true;
}

void StarVertex::render()
{
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
//	if (txImage != nullptr)
//		txImage->bind();
	glDrawArrays(GL_POINTS, 0, nStars);
	nStars  = 0;
}

void StarVertex::finish()
{

	render();

	flagStarted = false;

//	cout << "Total " << cStars << " rendered stars." << endl;

	switch (type) {
	case useSprites:
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		vbuf->unbind();

		glUseProgram(0);
		glDisable(GL_PROGRAM_POINT_SIZE);
		break;
	case usePoints:
	default:
		break;
	}
	type = useNotUsed;
}

void StarVertex::addStar(const vec3d_t &pos, const Color &color, double size)
{
	vertices[nStars].posStar = pos;
	vertices[nStars].color = color;
	vertices[nStars].size = size;

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
	objSize = ((srad / rdist) * 2.0) / pxSize;

//	if (star.getHIPNumber() == 0)
//		cout << "Sun distance: " << rdist << " size: " << glm::degrees(asin(srad/rdist) * 2.0)
//			 << " pixel: " << pxSize  << " -> " << srad / (rdist * pxSize) << endl;

	if (objSize > pxSize) {
		discSize = objSize;
		alpha = 1.0;
	} else {
		alpha  = faintestMag - appMag;
		discSize = baseSize;
		if (alpha > 1.0) {
			discScale = min(pow(2.0, 0.3 * (saturationMag - appMag)), 100.0);
			discSize *= discScale;
			alpha = 1.0;
		} else if (alpha < 0.0)
			alpha = 0.0;
	}

	color  = starColors->lookup(star.getTemperature());
	color.setAlpha(alpha);

//	if (star.getHIPNumber() == 0) {
//		cout << "Star size: " << discSize << " Position: " << rpos.x << "," << rpos.y << "," << rpos.z << endl;
//		cout << "Star color: " << color.getRed() << "," << color.getGreen() << "," << color.getBlue() << "," << color.getAlpha() << endl;
//		discSize = 20.0;
////		rpos = -rpos;
////		starBuffer->addStar(rpos, color, discSize);
//	}

//	if (spos == vec3d_t(0, 0, 0))
//		cout << "HIP " << star.getHIPNumber() << " at origin" << endl;

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

// Setup primary light sources for celestial stars
void Scene::setupPrimaryLightSources(const vector<const CelestialStar *> &nearStars,
		const vec3d_t &obs, double now, vector<LightSource> &ls)
{
	for (int idx = 0; idx < nearStars.size(); idx++) {
		const CelestialStar *star = nearStars[idx];
		LightSource ls;

		ls.spos        = star->getPosition(now) - (obs / KM_PER_PC);
		ls.luminosity = star->getLuminosity();
		ls.radius     = star->getRadius();
		ls.color      = starColors->lookup(star->getTemperature());

		lightSources.push_back(ls);
	}
}

// Setup secondary light sources for reflected objects
void Scene::setupSecondaryLightSources()
{
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

	starBuffer = new StarVertex(*this);
//	starBuffer->setTexture(starTexture);

	starRenderer = new StarRenderer();
	starRenderer->scene = this;
	starRenderer->context = &gl;
	starRenderer->starBuffer = starBuffer;
	starRenderer->starColors = starColors;
}

void Scene::initConstellations(const Universe &universe)
{
	const Constellations &constellations = universe.getAsterism();
	const StarCatalogue &starlib = *universe.getStarCatalogue();
	const vector<Asterism *> &asterisms = constellations.getAsterisms();

	pgmAsterism = smgr.createShader("line");

	pgmAsterism->use();

	vbufAsterism = new VertexBuffer(gl, 1);
	vbufAsterism->createBuffer(VertexBuffer::VBO, 1);

	for (int idx = 0; idx < asterisms.size(); idx++) {
		Asterism *aster = asterisms[idx];
		asterismLines += aster->hip.size();
	}
}

void Scene::renderStars(const StarCatalogue &starlib, const Player &player,
	double faintest)
{
	vec3d_t obs = player.getPosition();
	quatd_t rot = player.getOrientation();
	Camera *cam = player.getCamera(0);
	double  fov = cam->getFOV();
	double  aspect = gl.getAspect();

	starRenderer->cpos = cam->getPosition();
	starRenderer->pxSize = calculatePixelSize(cam);
	starRenderer->baseSize = 5.0; // default base star size
	starRenderer->faintestMag = faintestMag;
	starRenderer->faintestMagNight = faintest;
	starRenderer->saturationMag = saturationMag;
	starRenderer->starBuffer->start();

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
	const vector<Asterism *> &asterisms = constellations.getAsterisms();

	Camera *cam = player.getCamera(0);
	vec3d_t cpos = cam->getPosition();
	int cLines = 0;

	VertexLine *vertices = nullptr;
	uint32_t vbo = vbufAsterism->getVBO();

	pgmAsterism->use();
	vbufAsterism->bind();


	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, asterismLines * sizeof(VertexLine), nullptr, GL_STREAM_DRAW);
	vertices = reinterpret_cast<VertexLine *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	if (vertices == nullptr) {
		cerr << "Can't render stars - aborted (error code: " << glGetError() << ")" << endl;
		return;
	}

//	for (int idx = 0; idx < asterisms.size(); idx++) {
//		Asterism *aster = asterisms[idx];
//		cLines += aster->hip.size();
//	}
//	if (bufAsterism == nullptr) {
//		bufAsterism = new VertexLine[cLines];
//	}

	int rLines = 0;
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

			vertices[rLines].lpos   = vec3f_t((star1->getPosition(0) * KM_PER_PC) - cpos);
			vertices[rLines].color  = Color(0.5, 0.5, 0.5, 1.0);
			rLines++;
			vertices[rLines].lpos   = vec3f_t((star2->getPosition(0) * KM_PER_PC) - cpos);
			vertices[rLines].color  = Color(0.5, 0.5, 0.5, 1.0);
			rLines++;

//			std::cout << "HIP: " << aster->hip[sidx]
//					  << " Name: " << star->name(0) << std::endl;
		}
//		std::cout << std::endl;
	}

	if (!glUnmapBuffer(GL_ARRAY_BUFFER)) {
		cerr << "Buffer corrupted - aborted (error code: " << glGetError() << ")" << endl;
		return;
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexLine), (void *)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexLine), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	mat4f_t mvp = mat4f_t (prm.dmProj * prm.dmView * mat4d_t(1.0));

	uint32_t mvpLoc = glGetUniformLocation(pgmAsterism->getID(), "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_LINES, 0, rLines);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	vbufAsterism->unbind();

	glUseProgram(0);
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
