/*
 * font.h - TextureFont abstract package
 *
 *  Created on: Jan 14, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "render/gl/context.h"

class TextureFont
{
public:
	TextureFont(Context &gl) : gl(gl) {};
	virtual ~TextureFont() = default;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void render(char32_t ch) const = 0;
	virtual void render(const string &str) const = 0;

	virtual void render(char32_t ch, float xoff, float yoff) const = 0;
	virtual void render(const string &str, float xoff, float yoff) const = 0;

	virtual void setMaxAscent(int) = 0;
	virtual void setMaxDescent(int) = 0;

	virtual int getWidth(const string &str) const = 0;
	virtual int getMaxWidth() const = 0;
	virtual int getHeight() const = 0;
	virtual int getMaxAscent() const = 0;
	virtual int getMaxDescent() const = 0;

	virtual int getAdvance(char32_t ch) const = 0;

protected:
	Context &gl;
};
