/*
 * terrain.h - Terrain manager package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/render.h"
#include "render/mesh.h"

class TerrainManager;

class TerrainTile
{
    friend class TerrainManager;

public:
    TerrainTile(TerrainManager &mgr, uint32_t lod, uint32_t ilat, uint32_t ilng);
    ~TerrainTile();

    void load();
    void render(renderParameter &prm);

private:
    TerrainManager &tmgr;

    uint32_t lod;
    uint32_t ilat, ilng;
    tcrf_t   tcRange;

    Mesh *mesh = nullptr;
    Texture *texImage = nullptr;
    bool texOwn = false;
};

class TerrainManager
{
    friend class TerrainTile;

public:
    TerrainManager(Scene &scene);
    ~TerrainManager();

    void render(TerrainTile *tile, renderParameter &prm);
    void render(renderParameter &prm);

private:
    Scene &scene;

    ShaderProgram *pgm;

    TerrainTile *terrain[2];
};