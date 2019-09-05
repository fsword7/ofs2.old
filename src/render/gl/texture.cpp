/*
 * texture.cpp - Texture package
 *
 *  Created on: Aug 26, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/gl/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "util/stb_image.h"

Texture::Texture(uint32_t width, uint32_t height)
: width(width), height(height)
{
}

Texture::~Texture()
{
    if (data != nullptr)
        delete data;
    if (id > 0)
        glDeleteTextures(1, &id);
}

// void Texture::bind()
// {
//     if (loadFlag == false)
//         load();
//     glBindTexture(GL_TEXTURE_2D, id);
// }

void Texture::load()
{
    int target = GL_TEXTURE_2D;
    int bflag  = GL_CLAMP_TO_EDGE;

    glGenTextures(1, &id);
    bind();

    glTexParameteri(target, GL_TEXTURE_WRAP_S, bflag);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, bflag);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    if (mipMode == AutoMipMaps)
        glGenerateMipmap(target);
 
    loadFlag = true;
}

Texture *Texture::create(const string &fname)
{
    int      width, height, fcomps;
    int      comps = 3;
    uint8_t *data = nullptr;

    // stbi_set_flip_vertically_on_load(true);
	data = stbi_load(fname.c_str(), &width, &height, &fcomps, 0 /* comps */);
    if (data == nullptr) {
        // cout << "Can't open file: " << fname << endl;
        return nullptr;
    }

    // cout << "File: " << fname << endl;
    // cout << "Format Width " << width << " Height " << height << " Components " << fcomps << "(actual " << comps << ")" << endl;

    Texture *texImage = new Texture(width, height);
    texImage->components = comps;
    texImage->size = width * height * comps;
    texImage->data = new uint8_t[texImage->size];

    copy(data, data+texImage->size, texImage->data);
	stbi_image_free(data);

    // texImage->load();

    return texImage;
}