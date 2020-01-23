/*
 * truetype.h - Handling truetype fonts
 *
 *  Created on: Jan 23, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "font/font.h"

class TrueTypeFont : public TextureFont
{
public:
	TrueTypeFont() = default;
	~TrueTypeFont() = default;

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
};
