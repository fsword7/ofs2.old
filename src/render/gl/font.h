/*
 * font.h - OpenGL-based Font package using FreeType library
 *
 *  Created on: Feb 13, 2020
 *      Author: Tim Stark
 */

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "render/gl/context.h"

class TextureFont
{
private:
	struct Glyph
	{
		float ax, ay;	// Advance [X, Y]
		float bw, bh;	// Bitmap  [width, height]
		float bl, bt;	// Bitmap  [left, top]
		float tx, ty;	// Texture [X, Y]
	};

public:
	TextureFont(const Context &gl) : gl(gl) {}
	~TextureFont();

	static TextureFont *load(Context &gl, const fs::path &fname,
		int size = 12, int dpi = 96);

	static void ginit();
	static void gexit();

private:
	void initGlyphs();

private:
	const Context &gl;

	FT_Face face = nullptr;
	Glyph *glyph = nullptr;


	static FT_Library font;
};
