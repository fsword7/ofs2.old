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

static tcrf_t fullRange = { 0.0f, 1.0f, 0.0f, 1.0f };

TerrainTile::TerrainTile(TerrainManager &mgr, uint32_t lod, uint32_t ilat, uint32_t ilng)
: tmgr(mgr), lod(lod), ilat(ilat), ilng(ilng), tcRange(fullRange)
{
	center = calculateCenter();
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

	child = new TerrainTile(tmgr, nlod, nlat, nlng);
	if (child != nullptr)
		child->load();
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

	float latc = (PI/2) - PI * ((float(ilat)+0.5) / float(nlat));
	float lngc = (PI*2) * ((float(nlng/2 - ilng-1)+0.5) / float(nlng)) - PI;
    if (lngc < PI*2)
        lngc += PI*2;

	float slat = sin(latc), clat = cos(latc);
	float slng = sin(lngc), clng = cos(lngc);

	//	std::cout << "Index:  (" << lat << "," << lng << ") of (" << nlat << "," << nlng
	//              << ") at LOD " << lod+4 << std::endl;
	//	std::cout << "Center: (" << toDegrees(latc)
	//			  << "," << toDegrees(lngc) << ")" << std::endl;

	return vec3f_t(clat*clng, slat, clat*-slng);
}

void TerrainTile::load()
{
	state = Loading;

	string fname = fmt::sprintf("%s/surf/%02d/%06d/%06d.png",
		"data/systems/Sol/Earth/terrain/orbiter", lod+3, ilat, ilng);

	texImage = Texture::create(fname);
	texOwn = true;

	mesh = Mesh::createSphere(lod, ilat, ilng, 32, tcRange);
	if (mesh != nullptr) {
		mesh->allocate(tmgr.scene.getContext());
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

void TerrainManager::process(TerrainTile *tile, renderParameter &prm)
{
	int lod  = tile->lod;
	int nlat = 1 << tile->ilat;
	int nlng = 2 << tile->ilng;

	double trad0 = sqrt(2.0)*(PI/2);
	double trad, alpha, adist;
	double erad;
	double tdist, apr;
	int    tlod;
	bool   split = false;
    int    bias = 4;

	tile->state = TerrainTile::Rendering;

	// Find angle between camera and tile center
	// trad = trad0 / double(nlat);
	// alpha = acos(glm::dot(prm.cdir, tile->center));
	// adist = alpha - trad;

	// Check if tile is visible from camera position
	// If tile is hiding from camera position, mark
	// tile as invisible (LOD level 1+).
	// if (adist >= prm.viewap) {
	// 	std::cout << "Out of view: " << toDegrees(adist) << " >= " << toDegrees(prm.viewap) << std::endl;
    // 	tile->state = Tile::Invisible;
    // 	return;
	// }

	// Check if tile is visible in view

//
//	// Check LOD level from tile distance
//	{
////		erad = obj->getObject()->getRadius();
//		erad = 1.0;
//		if (adist < 0.0) {
//			tdist = prm.cdist - erad;
////            std::cout << "*** Above tile (LOD " << tile->lod+4 << "," << tile->lat << "," << tile->lng << ")"
////                      << std::endl;
//		} else {
//			double h = erad * sin(adist);
//			double a = prm.cdist - (erad * cos(adist));
//			tdist = sqrt(h*h + a*a);
//		}
//        bias -= 2.0 * sqrt(std::max(0.0, adist) / prm.viewap);
//		apr = tdist * prm.tanap;
//		if (apr > 0.000001)
//			tlod = std::max(0, std::min(prm.maxLOD, (int)(bias - log(apr)*1.1)));
//		else
//			tlod = prm.maxLOD;
//        tlod += prm.lodBias;
//		split = (lod < tlod+1);
//	}
//
////    split = false;
//	if (split == true) {
////        std::cout << "Tile split at LOD " << lod+4 << "(Expected LOD " << tlod+4 << ")" << std::endl;
//        bool valid = true;
//        // Check children that have valid flag
//		for (int idx = 0; idx < 4; idx++) {
//			Tile *child = tile->getChild(idx);
//			if (child == nullptr)
//				child = createChildNode(tile, idx);
//			else if (child->state == Tile::Invalid)
//				loader->queue(child);
//            if ((child->state & TILE_VALID) == 0)
//                valid = false;
//		}
//        // When all children have valid flags, process nodes in next LOD level.
//        if (valid) {
//            tile->state = Tile::Active;
//            for (int idx = 0; idx < 4; idx++)
//                processNode(tile->getChild(idx), prm);
//        }
//	} else {
////		std::cout << "Tile LOD Level: " << lod+4 << " (" << tile->lat << ","
////				  << tile->lng << ")" << std::endl;
////		std::cout << "Alpha: " << toDegrees(alpha) << " Distance: " << toDegrees(adist)
////				<< " Tile Distance: " << tdist << std::endl;
////		std::cout << "Aperture: " << apr << " LOD: " << lod+4
////				<< " Tile Center LOD: " << tlod+4 << std::endl;
//	}

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

	prm.model = glm::translate(glm::mat4(1.0f), vec3f_t(0.0f, 0.0f, 0.0f));
	prm.mvp = prm.mproj * prm.mview * prm.model;

	uint32_t mvpLoc = glGetUniformLocation(pgm->getID(), "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(prm.mvp));

	// Rendering terrain area
	for (int idx = 0; idx < 2; idx++)
		process(terrain[idx], prm);
	for (int idx = 0; idx < 2; idx++)
		render(terrain[idx], prm);

}