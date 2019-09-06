/*
 * terrain.cpp - Terrain manager package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/gl/texture.h"
#include "render/render.h"
#include "render/terrain.h"

// Global parameters
TerrainHandler *TerrainManager::loader = nullptr;

static tcrf_t fullRange = { 0.0f, 1.0f, 0.0f, 1.0f };

TerrainTile::TerrainTile(TerrainManager &mgr, uint32_t lod, uint32_t ilat, uint32_t ilng, TerrainTile *parent)
: Tree(parent), tmgr(mgr), lod(lod), ilat(ilat), ilng(ilng), tcRange(fullRange)
{
	center = calculateCenter();
	// cout << "    New tile: lod = " << lod << " ilat = " << ilat << " ilng = " << ilng << endl;

	mesh = Mesh::createSphere(lod, ilat, ilng, 32, tcRange);
	if (mesh != nullptr) {
		mesh->allocate(tmgr.scene.getContext());
		mesh->texImage = texImage;
	}
}

TerrainTile::~TerrainTile()
{
    if (mesh != nullptr)
        delete mesh;
}

TerrainTile *TerrainTile::createChild(int idx)
{
	TerrainTile *child = nullptr;

	int nlod = lod + 1;
	int nlat = ilat*2 + (idx / 2);
	int nlng = ilng*2 + (idx % 2);

	// cout << "Current tile: lod = " << lod << " ilat = " << ilat << " ilng = " << ilng << endl;
	// cout << "    New tile: lod = " << nlod << " ilat = " << nlat << " ilng = " << nlng << " index = " << idx << endl;

	child = new TerrainTile(tmgr, nlod, nlat, nlng, this);
	if (child != nullptr)
		tmgr.loader->queue(child);
	addChild(idx, child);

	return child;
}

void TerrainTile::split()
{
	TerrainTile *child = nullptr;

	for (int idx = 0; idx < 4; idx++) {
		if ((child = getChild(idx)) == nullptr)
			child = createChild(idx);
		else if (child->state == Invalid)
			child->load();
	}
}

vec3f_t TerrainTile::calculateCenter()
{
	int nlat = 1 << lod;
	int nlng = 2 << lod;

	float mlat0 = PI * float(ilat) / float(nlat);
	float mlat1 = PI * float(ilat+1) / float(nlat);
    // float mlng0 = PI*2 * (float(nlng/2 - ilng-1) / float(nlng)) - PI;
    // float mlng1 = PI*2 * (float(nlng/2 - ilng) / float(nlng)) - PI;
    float mlng0 = PI*2 * (float(ilng) / float(nlng)) - PI;
    float mlng1 = PI*2 * (float(ilng+1) / float(nlng)) - PI;

	// float latc = (PI/2) - PI * ((float(ilat)+0.5f) / float(nlat));
	// float lngc = (PI*2) * ((float(nlng/2 - ilng-1)+0.5) / float(nlng)) - PI;
 	// if (lngc < PI*2)
    //     lngc += PI*2;

 	float latc = PI * ((float(ilat)+0.5f) / float(nlat));
	float lngc = PI*2 * ((float(ilng)+0.5f) / float(nlng)) - PI;
 
	float slat = sin(latc), clat = cos(latc);
	float slng = sin(lngc), clng = cos(lngc);

	// cout << "Index:  (" << ilat << "," << ilng << ") of (" << nlat << "," << nlng
	//      << ") at LOD " << lod+3 << endl;
	// cout << "Center: (" << toDegree(latc)
	// 	 << "," << toDegree(lngc) << ")" << endl;
	// cout << "Coordinate: ( " << toDegree(mlng0) << " , " << toDegree(mlat0) << " ) - ( " 
	//                          << toDegree(mlng1) << " , " << toDegree(mlat0) << " )" << endl;
	// cout << "            ( " << toDegree(mlng0) << " , " << toDegree(mlat1) << " ) - ( " 
	//                          << toDegree(mlng1) << " , " << toDegree(mlat1) << " )" << endl;
						 
	// return vec3f_t(clat*clng, slat, clat*-slng);
	return vec3f_t(slat*clng, clat, slat*-slng);
}

void TerrainTile::setSubTexCoordRange(const tcrf_t &ptcr)
{
	// if ((ilng & 1) == 0) { // Right column
	if (ilng & 1) { // Right column
		tcRange.tumin = (ptcr.tumin + ptcr.tumax) / 2.0;
		tcRange.tumax = ptcr.tumax;
	} else {       // Left column
		tcRange.tumin = ptcr.tumin;
		tcRange.tumax = (ptcr.tumin + ptcr.tumax) / 2.0;
	}

	if (ilat & 1) { // Top row
		tcRange.tvmin = (ptcr.tvmin + ptcr.tvmax) / 2.0;
		tcRange.tvmax = ptcr.tvmax;
	} else {       // Bottom row
		tcRange.tvmin = ptcr.tvmin;
		tcRange.tvmax = (ptcr.tvmin + ptcr.tvmax) / 2.0;
	}
}

void TerrainTile::load()
{
	state = Loading;

	string fname = fmt::sprintf("%s/surf/%02d/%06d/%06d.jpg",
		"data/systems/Sol/Earth/terrain/orbiter", lod+3, ilat, ilng);

	texImage = Texture::create(fname);
	if (texImage == nullptr) {
    	// Non-existent tile. Have to load
    	// lower LOD tile from parent tile
    	// and set sub texture range.
    	TerrainTile *ptile = getParent();
    	if (ptile != nullptr) {
    		texImage = ptile->getTexture();
    		texOwn   = false;
    		setSubTexCoordRange(ptile->tcRange);
    		// std::cout << "Subtexture created" << std::endl;
    	}
	} else
		texOwn = true;

	mesh = Mesh::createSphere(lod, ilat, ilng, 32, tcRange);
	if (mesh != nullptr) {
		// mesh->allocate(tmgr.scene.getContext());
		mesh->texImage = texImage;
	}

	state = Inactive;
}

void TerrainTile::render(renderParameter &prm)
{
	if (mesh != nullptr)
		mesh->render(tmgr.scene.getContext(), prm);
}

// **************************************************

TerrainManager::TerrainManager(Scene &scene)
: scene(scene)
{
	ShaderManager &smgr = scene.getShaderManager();

	pgm = smgr.createShader("planet");

	// Initialize root of terrain tiles
	for (int idx = 0; idx < 2; idx++) {
		terrain[idx] = new TerrainTile(*this, 0, 0, idx);
		terrain[idx]->load();
	}
}

TerrainManager::~TerrainManager()
{
    // Delete all root tiles
	for (int idx = 0; idx < 2; idx++)
		delete terrain[idx];
}

void TerrainManager::ginit(Scene &scene)
{
	// Initialize and start up thread process for loading tiles
	loader = new TerrainHandler();
}

void TerrainManager::gexit()
{
	if (loader != nullptr) {
		delete loader;
		loader = nullptr;
	}
}

void TerrainManager::process(TerrainTile *tile, renderParameter &prm)
{
	int lod  = tile->lod;
	int nlat = 1 << tile->lod;
	int nlng = 2 << tile->lod;

	// farthest edge of quad tile radius
	constexpr double trad0 = sqrt(2.0)*(PI/2);

	double trad, alpha, adist;
	double erad;
	double tdist, apr;
	int    tlod;
	bool   splitFlag = false;
    int    bias = 4;

	tile->state = TerrainTile::Rendering;

	// Find angle between camera and tile center
	trad = trad0 / double(nlat);
	alpha = acos(glm::dot(prm.obj.cdir, -tile->center));
	adist = alpha - trad;
	
	// cout << "Tile Position:    (" << tile->center.x << "," << tile->center.y << "," << tile->center.z << ")" << endl;
	// cout << "Angle from center: " << toDegree(alpha) << endl;

	// Check if tile is visible from camera position
	// If tile is hiding from camera position, mark
	// tile as invisible (LOD level 1+).
	if (adist >= prm.obj.viewap) {
		// cout << "Out of view: " << tile->lod+3 << "," << tile->ilat << "," << tile->ilng << ") "
		// 	 << toDegree(adist) << " >= " << toDegree(prm.obj.viewap) << endl;
    	tile->state = TerrainTile::Invisible;
    	return;
	}
	// cout << "In View: (LOD " << tile->lod+3 << "," << tile->ilat << "," << tile->ilng << ") "
	// 	 << toDegree(adist) << " >= " << toDegree(prm.obj.viewap) << endl;

	// Check if tile is visible in view


	// Check LOD level from tile distance
	{
		erad = prm.obj.orad;
		if (adist < 0.0) {
			tdist = prm.obj.cdist - erad;
        	// cout << "*** Above tile (LOD " << tile->lod+3 << "," << tile->ilat << "," << tile->ilng << ")"
			// 	 << " Distance from ground " << erad << endl;
		} else {
			double h = erad * sin(adist);
			double a = prm.obj.cdist - (erad * cos(adist));
			tdist = sqrt(h*h + a*a);
		}
    	bias -= 2.0 * sqrt(std::max(0.0, adist) / prm.obj.viewap);
		apr = tdist * prm.tanap;
		if (apr > 0.000001)
			tlod = std::max(0, std::min(int(prm.obj.maxLOD), int(bias - log(apr)*1.1)));
		else
			tlod = prm.obj.maxLOD;
    	tlod += prm.obj.biasLOD;
		splitFlag = (lod < tlod+1);
	}

//    split = false;
	if (splitFlag == true) {
//        std::cout << "Tile split at LOD " << lod+4 << "(Expected LOD " << tlod+4 << ")" << std::endl;
       bool valid = true;
       // Check children that have valid flag

		for (int idx = 0; idx < 4; idx++) {
			TerrainTile *child = tile->getChild(idx);
			if (child == nullptr)
				child = tile->createChild(idx);
			else if (child->state == TerrainTile::Invalid)
				loader->queue(child);
           if ((child->state & TILE_VALID) == 0)
            	valid = false;
		}
       // When all children have valid flags, process nodes in next LOD level.
       if (valid) {
        	tile->state = TerrainTile::Active;
        	for (int idx = 0; idx < 4; idx++)
            	process(tile->getChild(idx), prm);
       }
	} else {
//		std::cout << "Tile LOD Level: " << lod+4 << " (" << tile->lat << ","
//				  << tile->lng << ")" << std::endl;
//		std::cout << "Alpha: " << toDegrees(alpha) << " Distance: " << toDegrees(adist)
//				<< " Tile Distance: " << tdist << std::endl;
//		std::cout << "Aperture: " << apr << " LOD: " << lod+4
//				<< " Tile Center LOD: " << tlod+4 << std::endl;
	}

}

void TerrainManager::render(TerrainTile *tile, renderParameter &prm)
{

	if (tile->state == TerrainTile::Rendering)
		tile->render(prm);
	else if (tile->state == TerrainTile::Active)
	{
		for (int idx = 0; idx < 4; idx++) {
			TerrainTile *child = tile->getChild(idx);
			if (child != nullptr && (child->state & TILE_ACTIVE))
				render(child, prm);
		}
	}
}

void TerrainManager::render(renderParameter &prm)
{
	pgm->use();

	prm.obj.maxLOD = 18;
	prm.obj.biasLOD = 0;

	// Object position and orientation parameters
	prm.obj.opos = vec3f_t(0.0f, 0.0f, -6.0f);
	prm.obj.orot = glm::rotate(mat4f_t(1.0f), glm::radians(0.0f), vec3f_t(0.0f, 0.0f, 1.0f));
	prm.obj.orad = 1.0f;

	// Camera position and oreintation parameters in object reference frame.
	prm.obj.cpos   = prm.obj.opos - prm.cpos;
	prm.obj.cdir   = glm::transpose(prm.obj.orot) * vec4f_t(prm.obj.cpos, 1.0f);
	prm.obj.cdist  = glm::length(prm.obj.cdir);
	prm.obj.viewap = (prm.obj.cdist >= prm.obj.orad) ? acos(prm.obj.orad / prm.obj.cdist) : 0.0f;

	// cout << "Terrain Manager - Render Parameter" << endl;
	// cout << "Planet Radius:    " << prm.obj.orad << endl;
	// cout << "Planet Position:  (" << prm.obj.opos.x << "," << prm.obj.opos.y << "," << prm.obj.opos.z << ")" << endl;
	// cout << "Camera Position:  (" << prm.obj.cpos.x << "," << prm.obj.cpos.y << "," << prm.obj.cpos.z << ")" << endl;
	// cout << "Camera Direction: (" << prm.obj.cdir.x << "," << prm.obj.cdir.y << "," << prm.obj.cdir.z << ")" << endl;
	// cout << "Camera Distance:  " << prm.obj.cdist << endl;
	// cout << "Horizon View:     " << toDegree(prm.obj.viewap) << endl;
	// cout << "Camera Position:  (" << prm.cpos.x << "," << prm.cpos.y << "," << prm.cpos.z
    //          << ") in Universe frame" << endl;

//    obj->getCoordinates(prm.cpos, &lat, &lng);
//    cout << "Planet Position:  (" << toDegrees(lat) << "," << toDegrees(lng) << ")" << endl;

	// prm.obj.cdist /= prm.obj.orad;
	prm.obj.cdir   = glm::normalize(prm.obj.cdir);

	prm.model = glm::translate(glm::transpose(prm.obj.orot), prm.obj.cpos);
	prm.mvp = prm.mproj * prm.mview * prm.model;

	uint32_t mvpLoc = glGetUniformLocation(pgm->getID(), "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(prm.mvp));

	// Rendering terrain area
	// for (int idx = 0; idx < 2; idx++)
	// 	process(terrain[idx], prm);
	// for (int idx = 0; idx < 2; idx++)
	// 	render(terrain[idx], prm);
	process(terrain[0], prm);
	render(terrain[0], prm);

}