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
class TerrainHandler;

class TerrainTile : public Tree<TerrainTile, QTREE_NODES>
{
    friend class TerrainManager;
    friend class TerrainHandler;

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

    TerrainTile(TerrainManager &mgr, uint32_t lod, uint32_t ilat, uint32_t ilng, TerrainTile *parent = nullptr);
    ~TerrainTile();

    inline Texture *getTexture() const { return texImage; }

    TerrainTile *createChild(int idx);

    vec3d_t calculateCenter();
    void setSubTexCoordRange(const tcrd_t &ptcr);
    void setWorldMatrix(renderParameter &prm);

    void split();
    void load();
    void render(renderParameter &prm);

private:
    TerrainManager &tmgr;

    TileState state = Invalid;

    uint32_t lod;
    uint32_t ilat, ilng;
    tcrd_t   tcRange;
    vec3d_t  center;
    
    Mesh *mesh = nullptr;
    Texture *texImage = nullptr;
    bool texOwn = false;
};

class TerrainHandler
{
public:
	TerrainHandler();
	~TerrainHandler();

	void start();
	void shutdown();

	void queue(TerrainTile *tile);
	bool unqueue(TerrainTile *tile);
	void unqueue(TerrainManager *mgr);

protected:
	void handle();

private:
	std::queue<TerrainTile *> tiles;

	// Thread process handle
	volatile bool runHandler;
	int           msFreq;
	std::thread   loader;
	std::mutex    mu_queue;
	std::mutex    mu_loading;
};

class TerrainManager
{
    friend class TerrainTile;

public:
    TerrainManager(Scene &scene, Object &object);
    ~TerrainManager();

    static void ginit(Scene &scene);
    static void gexit();

    void process(TerrainTile *tile, renderParameter &prm);
    void render(TerrainTile *tile, renderParameter &prm);
    void render(renderParameter &prm);

    Mesh *createSphere(int lod, int ilat, int ilng, int grids, tcrd_t &tcr);
    
private:
    Scene &scene;
    Object &body;

    ShaderProgram *pgm;

    static TerrainHandler *loader;

    TerrainTile *terrain[2];
};