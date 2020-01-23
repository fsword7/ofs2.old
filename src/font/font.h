/*
 * font.h - TextureFont abstract package
 *
 *  Created on: Jan 14, 2020
 *      Author: Tim Stark
 */

#pragma once

class TextureFont
{
public:
	TextureFont() = default;
	~TextureFont() = default;

	virtual void bind();
	virtual void unbind();

	virtual void render(char32_t ch) const;
	virtual void render(const string &str) const;

	virtual void render(char32_t ch, float xoff, float yoff) const;
	virtual void render(const string &str, float xoff, float yoff) const;

	virtual void setMaxAscent(int);
	virtual void setMaxDescent(int);

	virtual int getWidth(const string &str) const;
	virtual int getMaxWidth() const;
	virtual int getHeight() const;
	virtual int getMaxAscent() const;
	virtual int getMaxDescent() const;

	virtual int getAdvance(char32_t ch) const;
};
