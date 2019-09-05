// mesh.cpp - OFS mesh package
//
// Created on: August 19, 2019
//     Author: Tim Stark

#include "main/core.h"
#include "render/gl/context.h"
#include "render/gl/buffer.h"
#include "render/gl/texture.h"
#include "render/render.h"
#include "render/mesh.h"

Mesh::~Mesh()
{
    if (vbuf != nullptr)
        delete vbuf;
    if (vtx != nullptr)
        delete vtx;
    if (idx != nullptr)
        delete idx;
}

void Mesh::render(const Context *gl, renderParameter &prm)
{

//    pgm->use();
    if (isAllocated() == false)
        allocate(gl);
    vbuf->bind();

	vbuf->assign(VertexBuffer::VBO, vtx, nvtx*sizeof(vtxf_t));
    vbuf->assign(VertexBuffer::EBO, idx, nidx*sizeof(uint16_t));

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

    if (texImage != nullptr) {
        if (texImage->isLoaded() == false)
            texImage->load();
        glActiveTexture(GL_TEXTURE0);   
        texImage->bind();
    } else {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

	glDrawElements(GL_TRIANGLES, nidx, GL_UNSIGNED_SHORT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    vbuf->unbind();
}

void Mesh::allocate(const Context *ctx)
{

    vbuf = new VertexBuffer(*ctx, 1);

   	vbuf->createBuffer(VertexBuffer::VBO, 1);
	vbuf->createBuffer(VertexBuffer::EBO, 1);

	// vbuf->assign(VertexBuffer::VBO, vtx, nvtx*sizeof(vtxf_t));
    // vbuf->assign(VertexBuffer::EBO, idx, nidx*sizeof(uint16_t));

    allocatedFlag = true;
}

Mesh *Mesh::create(int nvtx, vtxf_t *vtx, int nidx, uint16_t *idx)
{
	Mesh *mesh = new Mesh();

	mesh->nvtx = nvtx;
	mesh->vtx  = vtx;
	mesh->nidx = nidx;
	mesh->idx  = idx;

	return mesh;
}

Mesh *Mesh::createSphere(int lod, int ilat, int ilng, int grids, tcrf_t &tcr)
{
	int nlat = 1 << lod;
	int nlng = 2 << lod;

	float mlat0 = PI * float(ilat) / float(nlat);
	float mlat1 = PI * float(ilat+1) / float(nlat);
    // float mlng0 = PI*2 * (float(nlng/2 - ilng-1) / float(nlng)) - PI;
    // float mlng1 = PI*2 * (float(nlng/2 - ilng) / float(nlng)) - PI;
    float mlng0 = PI*2 * (float(ilng) / float(nlng)) - PI;
    float mlng1 = PI*2 * (float(ilng+1) / float(nlng)) - PI;

    // cout << "ilng " << ilng << " nlng " << nlng << " mlng0 " << mlng0 << " mlng1 " << mlng1 << 
    //     " ( " << toDegree(mlng0) << " , " << toDegree(mlng1) << " )" << endl;

//    vObject *vobj = mgr->getVisualObject();
//	float   rad = vobj->getObject()->getRadius();
	float   rad, erad;

    float slng, clng;
    float slat, clat;
	float lng, lat;
    float tu, tv, du, dv;
    float tur, tvr;
    vec3f_t pos, nml;

    int      vidx;
    int      nVertices;
    int      nIndices, nIndices1;

    vtxf_t   *vtx;
    uint16_t *idx, *pidx;

    nVertices = (grids+1)*(grids+1);
    nIndices  = 6 * (grids*grids);
    vtx       = new vtxf_t[nVertices];
    idx       = new uint16_t[nIndices];

//    int      nTexCoords;
//    double *vtxs, *nmls;
//	double *pvtx, *pnml;
//    double   *tc, *ptc;
//    nVertices   = 3 * ((grids+1)*(grids+1));
//    nTexCoords  = 2 * ((grids+1)*(grids+1));
//    vtxs  = new double[nVertices];
//    nmls  = new double[nVertices];
//    tc    = new double[nTexCoords];
//    pvtx  = vtxs;
//    pnml  = nmls;
//    ptc   = tc;

    if (mlng0 < PI*2)
        mlng0 += PI*2;
    if (mlng1 < PI*2)
        mlng1 += PI*2;

//	std::cout << std::fixed << std::setprecision(10);
//	std::cout << "----------------------" << std::endl;
//	std::cout << "LOD:  " << lod << " nLAT: " << nlat << " nLNG: " << nlng
//			  << " Grids: " << grids << std::endl;
//	std::cout << "iLAT: " << ilat << " iLNG: " << ilng << std::endl;
//	std::cout << "Latitude Range:  " << toDegrees(mlat0) << " to " << toDegrees(mlat1) << std::endl;
//	std::cout << "Longitude Range: " << toDegrees(mlng0) << " to " << toDegrees(mlng1) << std::endl;

//	std::cout << "Delta Angle: " << dang << " U:" << du << std::endl;
//	std::cout << "Alpha: " << alpha << std::endl;

    du  = (mlng1 - mlng0) / grids;
    dv  = (mlat1 - mlat0) / grids;
    tur = tcr.tumax - tcr.tumin;
    tvr = tcr.tvmax - tcr.tvmin;

    vidx = 0;
	for (int y = 0; y <= grids; y++)
	{
		lat  = mlat0 + (mlat1-mlat0) * (float(y)/float(grids));
		slat = sin(lat); clat = cos(lat);
        tv = tcr.tvmin + tvr * (float(y)/float(grids));

//        std::cout << "Y = " << y << " LAT: " << toDegrees(lat) << std::endl;

		for (int x = 0; x <= grids; x++)
		{
			lng  = mlng0 + (mlng1-mlng0) * (float(x)/float(grids));
			slng = sin(lng); clng = cos(lng);
            tu   = tcr.tumin + tur * (float(x)/float(grids));

//            std::cout << "X = " << x << " LNG: " << toDegrees(lng) << std::endl;

//            pos = vec3d_t(axes.x()*slat*clng, axes.y()*clat, axes.z()*slat*slng);
//            nml = pos.normalized();

//            erad = rad + elevGlobe;
			erad = 1;
//            if (elev != nullptr) {
//            	int16_t edata = elev[(y+1)*ELEV_STRIDE + (x+1)];
//            	erad += (double(edata) * elevScale) / 1000.0;
//            	if (edata != 0)
//            		std::cout << "Elev X: " << x << " Y: " << y
//						      << "Elev: " << edata << std::endl;
//            }
//            else
//            	std::cout << "No elevation data for sphere..." << std::endl;

            nml = vec3f_t(slat*clng, clat, slat*-slng);

            pos = nml * erad;

            vtx[vidx].vx = pos.x;
            vtx[vidx].vy = pos.y;
            vtx[vidx].vz = pos.z;

            vtx[vidx].nx = nml.x;
            vtx[vidx].ny = nml.y;
            vtx[vidx].nz = nml.z;

            vtx[vidx].tu = tu;
            vtx[vidx].tv = tv;

            vidx++;

//			std::cout << "(" << x << "," << y << "): " << lng0 << " " << lat0 << " --> ";
//			std::cout << "(" << nml0.x() << "," << nml0.y() << "," << nml0.z() << ")" << std::endl;
		}
//		std::cout << std::endl;
//		std::cout << "Actual: " << pvtx - vtxs << " Total: " << nvtx << std::endl;
//		std::cout << "Estimate: " << (grids+1)*2*(grids-1)+2 << std::endl;

		// Degenerate triangles to connect
//		pvtx[0] = pvtx[3] = pvtx[-3];
//		pvtx[1] = pvtx[4] = pvtx[-2];
//		pvtx[2] = pvtx[5] = pvtx[-1];
//
//		pnml[0] = pnml[3] = pnml[-3];
//		pnml[1] = pnml[4] = pnml[-2];
//		pnml[2] = pnml[5] = pnml[-1];

	}
//	std::cout << "Vertices: Actual: " << vidx << " Expect: " << nVertices1 << std::endl;

    nIndices1 = 0;
    pidx = idx;
    for (int y = 0; y < grids; y++) {
        for (int x = 0; x < grids; x++) {
            *pidx++ = (y+0)*(grids+1) + (x+0);
            *pidx++ = (y+1)*(grids+1) + (x+0);
            *pidx++ = (y+0)*(grids+1) + (x+1);

            *pidx++ = (y+1)*(grids+1) + (x+0);
            *pidx++ = (y+1)*(grids+1) + (x+1);
            *pidx++ = (y+0)*(grids+1) + (x+1);

//            std::cout << "Index: (" << pidx[0] << "," << pidx[1] << "," << pidx[2] << ")" << std::endl;
//            std::cout << "Index: (" << pidx[3] << "," << pidx[4] << "," << pidx[5] << ")" << std::endl;
//
//            pidx += 6;
            nIndices1 += 6;
        }
    }

    return Mesh::create(vidx, vtx, nIndices, idx);
}