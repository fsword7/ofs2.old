/*
 * texture.cpp - Texture package
 *
 *  Created on: Aug 26, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/gl/texture.h"
#include "render/gl/dds.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Texture::Texture(uint32_t width, uint32_t height)
: width(width), height(height)
{
	compressed = false;
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

int Texture::getFormatComponents(int format)
{
	switch (format) {
		case GL_RGBA:
			return 4;
		case GL_RGB:
			return 3;
		case GL_LUMINANCE_ALPHA:
			return 2;
		case GL_ALPHA:
		case GL_INTENSITY:
		case GL_LUMINANCE:
			return 1;

		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			return format;

		default:
			return 0;
	}
}

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

    if (isCompressed())
    	glCompressedTexImage2D(target, 0, GL_RGB, width, height, 0, GL_UNSIGNED_BYTE, data);
    else
    	glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    if (mipMode == AutoMipMaps)
        glGenerateMipmap(target);
 
    loadFlag = true;
}

Texture *Texture::loadDDSFromMemory(uint8_t *data, uint32_t size)
{
    uint8_t *ptr = data;
    ddsHeader *hdr;
    int glFormat = -1;

    hdr = (ddsHeader *)ptr;

    // Check validation
    if (hdr->dwMagic != getFourCC("DDS ") ||
        hdr->dwSize != sizeof(ddsHeader) - sizeof(uint32_t)) {
        cerr << "*** Bad DDS header - aborted." << endl;
        return nullptr;
    }
    ptr += sizeof(ddsHeader);

    if (hdr->ddpf.dwFourCC != 0) {
        char fourCC[5];

        *(uint32_t *)&fourCC = hdr->ddpf.dwFourCC;
        fourCC[4] = '\0';

//        std::cout << "DDS Format Code: " << fourCC << std::endl;

        if (hdr->ddpf.dwFourCC == getFourCC("DXT1")) {
            glFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        } else if (hdr->ddpf.dwFourCC == getFourCC("DXT3")) {
            glFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        } else if (hdr->ddpf.dwFourCC == getFourCC("DXT5")) {
            glFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        } else {
            cerr << "*** Unknown DDS format code: " << fourCC << endl;
            return nullptr;
        }
    }

	Texture *texImage = new Texture(hdr->dwWidth, hdr->dwHeight);
	texImage->components = texImage->getFormatComponents(glFormat);
	texImage->size = hdr->dwWidth * hdr->dwHeight * texImage->components;
	texImage->data = new uint8_t[texImage->size];

	copy(ptr, ptr + texImage->size, texImage->data);

//    std::cout << "Image: " << hdr->dwWidth << " X " << hdr->dwHeight << " Depth: " << hdr->dwDepth
//              << "  MIP Levels: " << hdr->dwMipMapCount << std::endl;
//    std::cout << "  Size: " << img->getSize() << " bytes" << std::endl;

	return texImage;
}

Texture *Texture::loadDDSFromFile(const string& fname)
{
	uint8_t  *data;
	uint32_t  size;

	ifstream ddsFile(fname, ios::binary|ios::in);
	if (!ddsFile.is_open())
		return nullptr;

	ddsFile.seekg(0, ios::end);
	size = ddsFile.tellg();
	ddsFile.seekg(0, ios::beg);

	data = new uint8_t[size];
	ddsFile.read((char *)data, size);

	Texture *texImage = loadDDSFromMemory(data, size);
	delete []data;

	return texImage;
}

Texture *Texture::create(const string &fname)
{
	Texture *texImage = nullptr;

	{
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
	//		cout << "Bytes Per Pixel: " << (int)format->BytesPerPixel << endl;
		}

	//     cout << "File: " << fname << endl;
	//     cout << "Format Width " << image->w << " Height " << image->h << " Components " << (int)format->BytesPerPixel << endl;

		Texture *texImage = new Texture(image->w, image->h);
		texImage->components = format->BytesPerPixel;
		texImage->size = image->w * image->h * format->BytesPerPixel;
		texImage->data = new uint8_t[texImage->size];

		copy((uint8_t *)image->pixels, (uint8_t *)image->pixels + texImage->size, texImage->data);

		// texImage->load();

		SDL_FreeSurface(image);
	}

	return texImage;
}
