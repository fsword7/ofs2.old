/*
 * terrain.h - Terrain manager package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/mesh.h"

class TerrainManager;

class TerrainTile
{
    friend class TerrainManager;

public:
    TerrainTile(TerrainManager &mgr, uint32_t lod, uint32_t ilat, uint32_t ilng);
    ~TerrainTile() = default;

private:
    TerrainManager &tmgr;

    uint32_t lod;
    uint32_t ilat, ilng;

    Mesh *mesh = nullptr;
};

class TerrainManager
{
    friend class TerrainTile;

public:
    TerrainManager() = default;
    ~TerrainManager() = default;

private:
    TerrainTile *rootTiles[2];
};