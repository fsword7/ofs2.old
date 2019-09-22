/*
 * mesh.h - mesh rendering package
 *
 *  Created on: Aug 19, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/render.h"

class Context;
class VertexBuffer;
class Texture;

template <typename T>
struct Vertex32 {
    T vx, vy, vz;
    T nx, ny, nz;
    T tu, tv;
};

template <typename T>
struct Vertex64 {
    T vx, vy, vz;
    T ex, ey, ez;
    T nx, ny, nz;
    T tu, tv;
};

//typedef Vertex32<float> vtxf_t;
typedef Vertex64<float> vtxf_t;

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

public:
    int      nvtx = 0, nidx = 0;
    vtxf_t   *vtx = nullptr;
    uint16_t *idx = nullptr;

    bool allocatedFlag = false;
    VertexBuffer *vbuf = nullptr;
    Texture *texImage;
};
