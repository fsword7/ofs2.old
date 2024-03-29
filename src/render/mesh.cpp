/*
 * object.h - mesh rendering package
 *
 *  Created on: Aug 19, 2019
 *      Author: Tim Stark
 */

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
    if (isAllocated() == false)
        allocate(gl);
    vbuf->bind();

	vbuf->assign(VertexBuffer::VBO, vtx, nvtx*sizeof(vtxf_t));
    vbuf->assign(VertexBuffer::EBO, idx, nidx*sizeof(uint16_t));

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vtxf_t), (void *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vtxf_t), (void *)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vtxf_t), (void *)(6 * sizeof(float)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vtxf_t), (void *)(9 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	Color color = prm.obj.color;
    if (texImage != nullptr) {
        if (texImage->isLoaded() == false)
            texImage->load();
        glActiveTexture(GL_TEXTURE0);   
        texImage->bind();
    } else {
        glColor4f(color.getRed(), color.getGreen(),
        		  color.getBlue(), color.getAlpha());
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, nidx, GL_UNSIGNED_SHORT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    vbuf->unbind();

    glDisable(GL_CULL_FACE);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	vbuf->unbind();
}

void Mesh::allocate(const Context *ctx)
{

    vbuf = new VertexBuffer(*ctx, 1);

   	vbuf->createBuffer(VertexBuffer::VBO, 1);
	vbuf->createBuffer(VertexBuffer::EBO, 1);

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
