// mesh.h - OFS mesh package
//
// Created on: August 19, 2019
//     Author: Tim Stark

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
    ~Mesh() = default;

    void render(const Context *ctx) const;

 	static Mesh *create(int nvtx, vtxf_t *vtx, int nidx, uint16_t *idx);   
    static Mesh *createSphere(int lod, int ilat, int ilng, int grids, tcrf_t &tcr);

public:
    int      nvtx, nidx;
    vtxf_t   *vtx;
    uint16_t *idx;

};