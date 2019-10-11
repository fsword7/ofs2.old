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

#include "render/elevation.h"

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
        Invalid      = 0x0000,
        InQueue      = 0x0001,
        Loading      = 0x0002,
		Initializing = 0x0004,
        NoImage      = 0x0008,
        Inactive     = TILE_VALID,
        Active       = TILE_VALID|TILE_ACTIVE,
        Invisible    = TILE_VALID|TILE_ACTIVE|0x0100,
        Rendering    = TILE_VALID|TILE_ACTIVE|0x0200
    };

    TerrainTile(TerrainManager &mgr, uint32_t lod, uint32_t ilat, uint32_t ilng, TerrainTile *parent = nullptr);
    ~TerrainTile();

    inline Texture *getTexture() const { return texImage; }

    TerrainTile *createChild(int idx);

    vec3d_t calculateCenter();
    void setSubTexCoordRange(const tcrd_t &ptcr);

    void split();
    void load();
    void render(renderParameter &prm);

    // Elevation function calls
    int16_t *readElevation(int lod, int ilng, int ilat, double eres);
    bool     loadElevation();
    int16_t *getElevationData();

    inline bool     hasElevData() const { return elevOwn; }
    inline int16_t *getElevData() const { return elev; }

private:
    TerrainManager &tmgr;

    TileState state = Invalid;

    uint32_t lod;
    uint32_t ilat, ilng;
    vec3d_t  center;
    
    Mesh *mesh = nullptr;

    // Texture image parameters
    bool     texOwn = false;
    tcrd_t   tcRange;
    Texture *texImage = nullptr;

    // Elevation data parameters
    bool     elevOwn = false;
    int16_t *elev = nullptr;
    int16_t *ggelev = nullptr;
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
    TerrainManager(Scene &scene, const Object &object);
    ~TerrainManager();

    static void ginit(Scene &scene);
    static void gexit();

    void convertDoubleToTwoFloats(vec3d_t val, vec3f_t &high, vec3f_t &low);

    void process(TerrainTile *tile, renderParameter &prm);
    void render(TerrainTile *tile, renderParameter &prm);
    void render(renderParameter &prm);

    Mesh *createSphere(int lod, int ilat, int ilng, int grids, tcrd_t &tcr);
    
private:
    const Scene &scene;
    const Object &body;

    int      maxLOD;
    double   resElev;  // Elevation Resolution
    int      resGrid;  // Grid resolution

    ShaderProgram *pgm = nullptr;

    string surfFolder;

    static TerrainHandler *loader;

    TerrainTile *terrain[2];
};
