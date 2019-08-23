/*
 * terrain.cpp - Terrain manager package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/render.h"
#include "render/terrain.h"

static tcrf_t fullRange = { 0.0f, 1.0f, 0.0f, 1.0f };

TerrainTile::TerrainTile(TerrainManager &mgr, uint32_t lod, uint32_t ilat, uint32_t ilng)
: tmgr(mgr), lod(lod), ilat(ilat), ilng(ilng), tcRange(fullRange)
{
}

TerrainTile::~TerrainTile()
{
    if (mesh != nullptr)
        delete mesh;
}

void TerrainTile::load()
{
	mesh = Mesh::createSphere(lod, ilat, ilng, 32, tcRange);
	if (mesh != nullptr)
		mesh->allocate(tmgr.scene.getContext());
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

void TerrainManager::render(TerrainTile *tile, renderParameter &prm)
{

	// if (tile->state == TerrainTile::Rendering)
		tile->render(prm);
//	else if (tile->state == TerrainTfile::Active)
//	{
//		for (int idx = 0; idx < 4; idx++) {
//			TerrainTile *child = tile->getChild(idx);
//			if (child != nullptr && (child->state & TILE_ACTIVE))
//				render(child);
//		}
//	}
}

void TerrainManager::render(renderParameter &prm)
{
	pgm->use();

	prm.model = glm::translate(glm::mat4(1.0f), vec3f_t(0.0f, 0.0f, 0.0f));
	prm.mvp = prm.mproj * prm.mview * prm.model;

	uint32_t mvpLoc = glGetUniformLocation(pgm->getID(), "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(prm.mvp));

	// Rendering terrain area
	// for (int idx = 0; idx < 2; idx++)
	// 	process(terrain[idx], prm);
	for (int idx = 0; idx < 2; idx++)
		render(terrain[idx], prm);

}