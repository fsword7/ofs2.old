/*
 * terrain.cpp - Terrain manager package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/render.h"
#include "render/terrain.h"

TerrainTile::TerrainTile(TerrainManager &mgr, uint32_t lod, uint32_t ilat, uint32_t ilng)
: tmgr(mgr), lod(lod), ilat(ilat), ilng(ilng)
{
}

TerrainTile::~TerrainTile()
{
    if (mesh != nullptr)
        delete mesh;
}

void TerrainTile::load()
{
}

// **************************************************

TerrainManager::TerrainManager()
{

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

void TerrainManager::render(renderParameter &prm)
{
}