/*
 * font.h - OpenGL-based Font package using FreeType library
 *
 *  Created on: Feb 13, 2020
 *      Author: Tim Stark
 */

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

class TextureFont
{
private:
	struct Glyph
	{
		char32_t ch;
		GLuint glName;

		float ax, ay;	// Advance [X, Y]
		float bw, bh;	// Bitmap  [width, height]
		float bl, bt;	// Bitmap  [left, top]
		float tx, ty;	// Texture [X, Y]
	};

public:
	TextureFont(Context &gl) : gl(gl) {}
	~TextureFont();

	static TextureFont *load(Context &gl, const fs::path &fname,
		int size = 12, int dpi = 96);

	static void ginit();
	static void gexit();

	void render(const string &text, float x, float y, const Color &color);

private:
	void computeTextureSize();
	void buildAtlas();
	void initGlyphs();

private:
	Context &gl;

	ShaderProgram *pgm = nullptr;
	GLuint vao, vbo;

	FT_Face face = nullptr;
	Glyph *glyph = nullptr;

	// Atlas texture
	int maxTextureSize = 0;
	int texWidth = 0;
	int texHeight = 0;

	static FT_Library font;
};
