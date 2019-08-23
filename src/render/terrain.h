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
    ~TerrainTile();

    void load();

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
    TerrainManager();
    ~TerrainManager();

    void render(renderParameter &prm);
    
private:
    TerrainTile *terrain[2];
};