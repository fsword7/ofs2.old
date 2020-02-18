/*
 * overlay.h - overlay/headup package
 *
 *  Created on: Feb 18, 2020
 *      Author: Tim Stark
 */

#pragma once

class TextureFont;
class Overlay
{
public:
	Overlay(Scene &render) : render(render) {}
	~Overlay() = default;

	void setFont(TextureFont *font);
	void setPosition(float x, float y);

private:
	Scene &render;

	TextureFont *font = nullptr;
	bool fontChanged = false;

	float xoff = 0.0f;
	float yoff = 0.0f;
};
