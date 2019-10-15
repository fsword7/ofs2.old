/*
 * texture.cpp - Texture package
 *
 *  Created on: Aug 26, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/gl/texture.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);

	SDL_Surface *image = IMG_Load(fname.c_str());
	if (image == nullptr)
		return nullptr;

	SDL_PixelFormat *format = image->format;
	int glFormat = -1;

	if (format->BytesPerPixel == 4) {
		if (format->Rshift == 24 && format->Aloss == 0)
			glFormat = GL_ABGR_EXT;
		else if(format->Rshift == 16 && format->Aloss == 8)
			glFormat = GL_BGRA;
		else if (format->Rshift == 16 && format->Ashift == 24)
			glFormat = GL_BGRA;
		else if (format->Rshift == 0 && format->Ashift == 24)
			glFormat = GL_RGBA;
	} else if (format->BytesPerPixel == 3) {
		if (format->Rshift == 16)
			glFormat = GL_BGR;
		else if (format->Rshift == 0)
			glFormat = GL_RGB;
	} else {
		cout << "Bytes Per Pixel: " << (int)format->BytesPerPixel << endl;
	}

     cout << "File: " << fname << endl;
     cout << "Format Width " << image->w << " Height " << image->h << " Components " << (int)format->BytesPerPixel << endl;

    Texture *texImage = new Texture(image->w, image->h);
    texImage->components = format->BytesPerPixel;
    texImage->size = image->w * image->h * format->BytesPerPixel;
    texImage->data = new uint8_t[texImage->size];

    copy((uint8_t *)image->pixels, (uint8_t *)image->pixels + texImage->size, texImage->data);

    // texImage->load();

	IMG_Quit();
	SDL_FreeSurface(image);

    return texImage;
}
