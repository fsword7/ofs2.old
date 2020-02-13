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
	if (glyph != nullptr)
		delete [] glyph;
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

void TrueTypeFont::initGlyphs()
{
	FT_GlyphSlot slot = face->glyph;

	glyph = new Glyph[face->num_glyphs];
	for (int gidx = 0; gidx < face->num_glyphs; gidx++)
	{
		FT_Load_Glyph(face, gidx, FT_LOAD_RENDER);

		glyph[gidx].ch = 0;
		glyph[gidx].ax = slot->advance.x >> 6;
		glyph[gidx].ay = slot->advance.y >> 6;
		glyph[gidx].bw = slot->bitmap.width;
		glyph[gidx].bh = slot->bitmap.rows;
		glyph[gidx].bl = slot->bitmap_left;
		glyph[gidx].bt = slot->bitmap_top;
	}

	// Assign UNICODE code to glyph table
	uint32_t gidx;
	char32_t ch = FT_Get_First_Char(face, &gidx);
	while ( gidx != 0) {
//		cout << fmt::sprintf("Glyph index %d: %08X\n", gidx, uint32_t(ch));
		glyph[gidx].ch = ch;
		ch = FT_Get_Next_Char(face, ch, &gidx);
	}
}

void TrueTypeFont::computeTextureSize()
{
	FT_GlyphSlot slot = face->glyph;

	int roww = 0;
	int rowh = 0;
	int w = 0;
	int h = 0;

	for (int gidx = 0; gidx < face->num_glyphs; gidx++) {
		if (glyph[gidx].ch == 0)
			continue;
		if (roww + glyph[gidx].bw + 1 >= maxTextureSize) {
			w = max(w, roww);
			h += rowh;
			roww = 0;
			rowh = 0;
		}
		roww += glyph[gidx].bw + 1;
		rowh = max(rowh, (int)glyph[gidx].bh);
	}

	w = max(w, roww);
	h += rowh;

	texWidth = w;
	texHeight = h;

	cout << fmt::sprintf("Texture size: (%d,%d)\n", texWidth, texHeight);
}

bool TrueTypeFont::initAtlas()
{
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
//	cout << fmt::sprintf("Maximum texture size: %d\n", maxTextureSize);

	initGlyphs();
	computeTextureSize();

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
	font->initAtlas();

	return font;
}
