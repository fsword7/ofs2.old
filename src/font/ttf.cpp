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

TrueTypeFont *TrueTypeFont::load(const string &fileName, int size, int dpi)
{
	FT_Face face;

	if (font == nullptr) {
		if (FT_Init_FreeType(&font)) {
			cerr << "TTF: Can't initialize freetype library - aborted." << endl;
			return nullptr;
		}
	}

	if (FT_New_Face(font, fileName.c_str(), 0, &face) != 0) {
		fmt::fprintf(cerr, "TTF: Can't open font '%s' - aborted: %s\n",
			fileName, strerror(errno));
		return nullptr;
	}

	if (!FT_IS_SCALABLE(face)) {
		fmt::fprintf(cerr, "TTF: Font '%s' is not scalable - aborted\n",
			fileName);
		return nullptr;
	}

	if (FT_Set_Char_Size(face, 0, size << 6, dpi, dpi) != 0) {
		fmt::fprintf(cerr, "TTF: Can't set %i on font '%s' - aborted\n",
			size, fileName);
	}

	return nullptr;
}
