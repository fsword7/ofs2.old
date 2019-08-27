/*
 * texture.h - Texture package
 *
 *  Created on: Aug 26, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/gl/context.h"

class Texture
{
public:

    enum MipMapMode {
		NoMipMaps = 0,
		FixedMipMaps = 1,
		AutoMipMaps = 2
	};

    Texture(uint32_t width, uint32_t height);
    ~Texture();

    inline void bind() const   { glBindTexture(GL_TEXTURE_2D, id); }
    inline void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

    void load();

private:
    uint32_t width, height;

    uint8_t *data = nullptr;

    MipMapMode mipMode = NoMipMaps;
    GLuint id = 0;
};