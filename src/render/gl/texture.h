/*
 * texture.h - Texture package
 *
 *  Created on: Aug 26, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/gl/context.h"

namespace ofs::gl {

};

class Texture
{
public:
    enum MipMapMode {
		NoMipMaps = 0,
		FixedMipMaps = 1,
		AutoMipMaps = 2
	};

    enum BorderMode {
        Wrap = 0,
        BorderClamp = 1,
        EdgeClamp = 2
    };

    Texture(uint32_t width, uint32_t height);
    ~Texture();

    inline bool isLoaded() const { return loadFlag; }
    inline void bind() const     { glBindTexture(GL_TEXTURE_2D, id); }
    inline void unbind() const   { glBindTexture(GL_TEXTURE_2D, 0); }

    // void bind();
    void load();

    static Texture *create(const string &fname);

private:
    uint32_t width, height;
    uint32_t size;

    uint32_t components = 0;

    uint8_t *data = nullptr;

    MipMapMode mipMode = NoMipMaps;
    BorderMode borderMode = EdgeClamp;

    bool loadFlag = false;
    GLuint id = 0;
};