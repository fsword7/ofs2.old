/*
 * font.cpp - OpenGL-based Font package using FreeType library
 *
 *  Created on: Feb 13, 2020
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/gl/font.h"

FT_Library TextureFont::font = nullptr;

TextureFont::~TextureFont()
{
	if (face != nullptr)
		FT_Done_Face(face);
	if (glyph != nullptr)
		delete [] glyph;
}

void TextureFont::ginit()
{
	if (font != nullptr)
		return;

	if (FT_Init_FreeType(&font)) {
		cerr << "TTF: Can't initialize freetype library - aborted." << endl;
	}
}

void TextureFont::gexit()
{
	if (font != nullptr)
		FT_Done_FreeType(font);
}

void TextureFont::initGlyphs()
{
}

TextureFont *TextureFont::load(Context &gl, const fs::path &path, int size, int dpi)
{
	FT_Face face;

	if (FT_New_Face(font, path.string().c_str(), 0, &face) != 0) {
		fmt::fprintf(cerr, "TTF: Can't open font %s: %s\n",
			path, strerror(errno));
		return nullptr;
	}

	if (!FT_IS_SCALABLE(face)) {
		fmt::fprintf(cerr, "TTF: Font %s is not scalable\n",
			path);
		FT_Done_Face(face);
		return nullptr;
	}

	if (FT_Set_Char_Size(face, 0, size << 6, dpi, dpi) != 0) {
		fmt::fprintf(cerr, "TTF: Can't set %i on font %s\n",
			size, path);
		FT_Done_Face(face);
		return nullptr;
	}

	if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0) {
		fmt::fprintf(cerr, "TTF: Unicode not supported on font %s\n",
			path);
		FT_Done_Face(face);
		return nullptr;
	}

	fmt::fprintf(cout, "TTF: Loading %s (%s) %d glyphs...\n",
		face->family_name, face->style_name, face->num_glyphs);


	TextureFont *font = new TextureFont(gl);

	// Initializing glyph database from font files
	font->face = face;
	font->initGlyphs();

	return font;
}
