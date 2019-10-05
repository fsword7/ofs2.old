/*
 * buffer.h
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/context.h"

namespace ofs::gl {

};

class VertexBuffer
{
public:
    enum BufferType {
        VAO, VBO, EBO
    };

    VertexBuffer(const Context &gl, int nArrays = 1);
    ~VertexBuffer();

    inline void bind() const { glBindVertexArray(vao); };
    inline void unbind() const { glBindVertexArray(0); };

    uint32_t createBuffer(BufferType type, uint32_t nBuffers);
    void assign(BufferType type, void *data, uint32_t size);

private:
    const Context &gl;

    GLuint vao, vbo, ebo;
};
