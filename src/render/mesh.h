// mesh.h - OFS mesh package
//
// Created on: August 19, 2019
//     Author: Tim Stark

#pragma once

#include "render/render.h"

class Context;
class VertexBuffer;
class Texture;

template <typename T>
struct Vertex {
    T vx, vy, vz;
    T nx, ny, nz;
    T tu, tv;
};

typedef Vertex<float> vtxf_t;

// Texture Coordination Range
template <typename T>
struct tcRange
{
	T tumin, tumax;
	T tvmin, tvmax;
};

typedef tcRange<float>  tcrf_t;
typedef tcRange<double> tcrd_t;

class Mesh
{
public:
    Mesh() = default;
    ~Mesh();

    inline bool isAllocated() const { return allocatedFlag; }

    void allocate(const Context *ctx);
    void render(const Context *ctx, renderParameter &prm);

 	static Mesh *create(int nvtx, vtxf_t *vtx, int nidx, uint16_t *idx);   
    static Mesh *createSphere(int lod, int ilat, int ilng, int grids, tcrf_t &tcr);

public:
    int      nvtx = 0, nidx = 0;
    vtxf_t   *vtx = nullptr;
    uint16_t *idx = nullptr;

    bool allocatedFlag = false;
    VertexBuffer *vbuf = nullptr;
    Texture *texImage;
};