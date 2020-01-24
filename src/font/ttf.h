/*
 * truetype.h - Handling truetype fonts
 *
 *  Created on: Jan 23, 2020
 *      Author: Tim Stark
 */

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "font/font.h"

class TrueTypeFont : public TextureFont
{
private:
	struct Glyph
	{
		wchar_t ch;

		float ax, ay; // Advance [X, Y]
		float bw, bh; // Bitmap [width, height]
		float bl, bt; // Bitmap [left, top]
		float tx, ty; // Texture Offset [X, Y]
	};

public:
	TrueTypeFont(Context &gl) : TextureFont(gl) {};
	~TrueTypeFont();

	static TextureFont *load(Context &gl, const fs::path &fileName,
		int size = 12, int dpi = 96);

	void bind() override;
	void unbind() override;

	void setMaxAscent(int) override;
	void setMaxDescent(int) override;

	int getAdvance(char32_t ch) const override;

	void render(char32_t ch) const override;
	void render(const string &str) const override;

	void render(char32_t ch, float xoff, float yoff) const override;
	void render(const string &str, float xoff, float yoff) const override;

	int getWidth(const string &str) const override;
	int getMaxWidth() const override;
	int getHeight() const override;
	int getMaxAscent() const override;
	int getMaxDescent() const override;

private:
	bool loadGlyph(wchar_t ch, Glyph &glyph);
	void initGlyphs();
	bool initAtlas();

private:
	FT_Face face;
	vector<Glyph> glyphs;

	static FT_Library font;
};
