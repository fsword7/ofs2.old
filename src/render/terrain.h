/*
 * terrain.h - Terrain manager package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/render.h"
#include "render/mesh.h"
#include "util/tree.h"

#define TILE_VALID  0x80000000
#define TILE_ACTIVE 0x40000000

class TerrainManager;

class TerrainTile : public Tree<TerrainTile, QTREE_NODES>
{
    friend class TerrainManager;

public:
    enum TileState {
        Invalid   = 0x0000,
        InQueue   = 0x0001,
        Loading   = 0x0002,
        NoImage   = 0x0004,
        Inactive  = TILE_VALID,
        Active    = TILE_VALID|TILE_ACTIVE,
        Invisible = TILE_VALID|TILE_ACTIVE|0x0008,
        Rendering = TILE_VALID|TILE_ACTIVE|0x0010
    };

    TerrainTile(TerrainManager &mgr, uint32_t lod, uint32_t ilat, uint32_t ilng);
    ~TerrainTile();

    TerrainTile *createChild(int idx);

    vec3f_t calculateCenter();

    void split();
    void load();
    void render(renderParameter &prm);

private:
    TerrainManager &tmgr;

    TileState state = Invalid;

    uint32_t lod;
    uint32_t ilat, ilng;
    tcrf_t   tcRange;
    vec3f_t  center;
    
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

    void process(TerrainTile *tile, renderParameter &prm);
    void render(TerrainTile *tile, renderParameter &prm);
    void render(renderParameter &prm);

private:
    Scene &scene;

    ShaderProgram *pgm;

    TerrainTile *terrain[2];
};