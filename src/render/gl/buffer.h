/*
 * buffer.h
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/gl/context.h"

class VertexBuffer
{
public:
    enum BufferType {
        VAO, VBO, EBO
    };

    VertexBuffer(Context &gl, int nArrays = 1);
    ~VertexBuffer();

    inline void bind() const { glBindVertexArray(vao); };
    inline void unbind() const { glBindVertexArray(0); };

    void createBuffer(BufferType type, uint32_t nBuffers);
    void assign(BufferType type, void *data, uint32_t size);

private:
    Context &gl;

    GLuint vao, vbo, ebo;
};