/*
 * terrain.cpp - Terrain manager package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/terrain.h"

TerrainTile::TerrainTile(TerrainManager &mgr, uint32_t lod, uint32_t ilat, uint32_t ilng)
: tmgr(mgr), lod(lod), ilat(ilat), ilng(ilng)
{

}