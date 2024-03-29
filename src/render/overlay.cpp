/*
 * overlay.cpp - overlay/headup panel package
 *
 *  Created on: Feb 18, 2020
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/render.h"
#include "render/overlay.h"

OverlayBuffer::OverlayBuffer()
{
	setbuf(nullptr, 0);
}

Overlay::Overlay(Scene *render)
: ostream(&obuf), render(render)
{
	obuf.set(this);
	reset();
}

void Overlay::setFont(TextureFont *_font)
{
	if (font != nullptr)
		fontChanged = true;
	font = _font;
}

void Overlay::setColor(const Color &_color)
{
	color = _color;
}

void Overlay::setPosition(float x, float y)
{
	xoff = x;
	yoff = y;
}

void Overlay::reset()
{
	width  = render->getWidth();
	height = render->getHeight();

	home();
}

void Overlay::home()
{
	xoff = 0.0;
	yoff = height;
}

void Overlay::moveBy(float x, float y)
{
	xoff += x;
	yoff -= y;
}

void Overlay::print(const string &text)
{
	yoff -= font->getHeight() + ySpace;
	font->render(text, xoff, yoff, color);
}
