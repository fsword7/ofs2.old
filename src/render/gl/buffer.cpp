/*
 * buffer.cpp
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/gl/buffer.h"

VertexBuffer::VertexBuffer(const Context &gl, int nArrays)
: gl(gl), vao(0), vbo(0), ebo(0)
{
    glGenVertexArrays(nArrays, &vao);
}

VertexBuffer::~VertexBuffer()
{
    if (vbo > 0)
        glDeleteBuffers(1, &vbo);
    if (ebo > 0)
        glDeleteBuffers(1, &ebo);
    if (vao > 0)
        glDeleteVertexArrays(1, &vao);
}

void VertexBuffer::createBuffer(BufferType type, uint32_t nBuffers)
{
	bind();
	switch (type) {
	case VBO:
		glGenBuffers(nBuffers, &vbo);
		break;
	case EBO:
		glGenBuffers(nBuffers, &ebo);
		break;
	}
}

void VertexBuffer::assign(BufferType type, void *data, uint32_t size)
{
    bind();
	switch (type) {
	case VBO:
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
		break;
	case EBO:
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
		break;
	}
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // unbind();
}
