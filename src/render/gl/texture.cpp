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
    glGenTextures(1, &id);
}

Texture::~Texture()
{
    if (data != nullptr)
        delete data;
    glDeleteTextures(1, &id);
}

void Texture::load()
{
    int target = GL_TEXTURE_2D;
    int bflag  = GL_CLAMP_TO_EDGE;

    bind();

    glTexParameteri(target, GL_TEXTURE_WRAP_S, bflag);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, bflag);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    if (mipMode == AutoMipMaps)
        glGenerateMipmap(target);
}

Texture *Texture::create(const string &fname)
{
    int      width, height;
    int      components;
    uint8_t *data = nullptr;

    stbi_set_flip_vertically_on_load(true);
	data = stbi_load(fname.c_str(), &width, &height, &components, 0);
    if (data == nullptr) {
        cout << "Can't open file: " << fname << endl;
        return nullptr;
    }

    cout << "File: " << fname << endl;
    cout << "Format Width " << width << " Height " << height << " Components " << components << endl;

    Texture *texImage = new Texture(width, height);
    texImage->components = components;
    texImage->size = width * height * components;
    texImage->data = new uint8_t[texImage->size];

    copy(data, data+texImage->size, texImage->data);
	stbi_image_free(data);

    texImage->load();

    return texImage;
}