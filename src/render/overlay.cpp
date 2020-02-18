/*
 * overlay.cpp - overlay/headup panel package
 *
 *  Created on: Feb 18, 2020
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/render.h"
#include "render/overlay.h"

void Overlay::setFont(TextureFont *_font)
{
	if (font != nullptr)
		fontChanged = true;
	font = _font;
}

void Overlay::setPosition(float x, float y)
{
	xoff = x;
	yoff = y;
}

