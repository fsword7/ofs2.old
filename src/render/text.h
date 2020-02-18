/*
 * text.h - text message handler package (foreground overlay screen)
 *
 *  Created on: Jan 23, 2020
 *      Author: Tim Stark
 */

#pragma once

class TextureFont;

class Text
{
public:
	Text() = default;
	~Text() = default;

	void begin();
	void end();

	inline void setFont(TextureFont *_font) { font = _font; }

private:
	TextureFont *font = nullptr;
};
