/*
 * ttf.cpp - Truetype font package
 *
 *  Created on: Jan 23, 2020
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "font/ttf.h"

FT_Library TrueTypeFont::font = nullptr;

TrueTypeFont::~TrueTypeFont()
{
	if (face != nullptr)
		FT_Done_Face(face);
}

void TrueTypeFont::bind()
{
}

void TrueTypeFont::unbind()
{
}

void TrueTypeFont::setMaxAscent(int)
{
}
void TrueTypeFont::setMaxDescent(int)
{
}

int TrueTypeFont::getAdvance(char32_t ch) const
{
	return 0;
}

void TrueTypeFont::render(char32_t ch) const
{
}
void TrueTypeFont::render(const string &str) const
{
}

void TrueTypeFont::render(char32_t ch, float xoff, float yoff) const
{
}
void TrueTypeFont::render(const string &str, float xoff, float yoff) const
{
}


int TrueTypeFont::getWidth(const string &str) const
{
	return 0;
}

int TrueTypeFont::getMaxWidth() const
{
	return 0;
}

int TrueTypeFont::getHeight() const
{
	return 0;
}

int TrueTypeFont::getMaxAscent() const
{
	return 0;
}

int TrueTypeFont::getMaxDescent() const
{
	return 0;
}

bool TrueTypeFont::loadGlyph(wchar_t ch, Glyph &glyph)
{
	FT_GlyphSlot slot = face->glyph;

	if (FT_Load_Char(face, ch, FT_LOAD_RENDER) != 0)
	{
		glyph.ch = 0;
		return false;
	}

	glyph.ch = ch;
	glyph.ax = slot->advance.x >> 6;
	glyph.ay = slot->advance.y >> 6;
	glyph.bw = slot->bitmap.width;
	glyph.bh = slot->bitmap.rows;
	glyph.bl = slot->bitmap_left;
	glyph.bt = slot->bitmap_top;

	return true;
}

void TrueTypeFont::initGlyphs()
{
	FT_GlyphSlot slot = face->glyph;

	// Initial 256 glyphs reserved
	glyphs.reserve(face->num_glyphs);
	for (int idx = 0; idx < face->num_glyphs; idx++)
	{
		FT_Load_Glyph(face, idx, FT_LOAD_RENDER);

		glyphs[idx].ax = slot->advance.x >> 6;
		glyphs[idx].ay = slot->advance.y >> 6;
		glyphs[idx].bw = slot->bitmap.width;
		glyphs[idx].bh = slot->bitmap.rows;
		glyphs[idx].bl = slot->bitmap_left;
		glyphs[idx].bt = slot->bitmap_top;
	}

}

bool TrueTypeFont::initAtlas()
{
	initGlyphs();

	return false;
}

TextureFont *TrueTypeFont::load(Context &gl, const fs::path &path, int size, int dpi)
{
	FT_Face face;

	if (font == nullptr) {
		if (FT_Init_FreeType(&font)) {
			cerr << "TTF: Can't initialize freetype library - aborted." << endl;
			return nullptr;
		}
	}

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


	TrueTypeFont *font = new TrueTypeFont(gl);
	font->face = face;

	// Initializing atlas database
//	font->initAtlas();

	return font;
}
