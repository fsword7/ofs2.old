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
public:
	TrueTypeFont(Context &gl) : TextureFont(gl) {};
	~TrueTypeFont();

	static TextureFont *load(Context &gl, const string &fileName, int size, int dpi);

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
	FT_Face face;

	static FT_Library font;
};
