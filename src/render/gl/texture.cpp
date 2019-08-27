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
    bind();

    // glTexParameteri(target, GL_TEXTURE_WRAP_S, borderFlag);
    // glTexParameteri(target, GL_TEXTURE_WRAP_T, borderFlag);
    // glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(target, GL_TEXTURE_MIN_FILTER,
    // 	mipFlag ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    if (mipMode == AutoMipMaps)
        glGenerateMipmap(GL_TEXTURE_2D);
}