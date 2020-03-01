/*
 * overlay.h - overlay/headup package
 *
 *  Created on: Feb 18, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "render/gl/context.h"
#include "render/gl/font.h"

class TextureFont;

//class OverlayBuffer : public streambuf
//{
//public:
//	OverlayBuffer();
//};

class Overlay // : public ostream
{
public:
//	Overlay(Scene *render) : render(render) {}
	Overlay(Scene *render);
	~Overlay() = default;

	void setFont(TextureFont *font);
	void setPosition(float x, float y);
	void setColor(const Color &color);

	void reset();
	void home();
	void moveBy(float x, float y);

	void print(const string &text);

private:
	Scene *render = nullptr;

	float width, height;

	TextureFont *font = nullptr;
	bool fontChanged = false;
	Color color;

	float xoff   = 0.0f;
	float yoff   = 0.0f;
	float ySpace = 3.0f;
};
