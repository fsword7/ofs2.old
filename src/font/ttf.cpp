/*
 * ttf.cpp - Truetype font package
 *
 *  Created on: Jan 23, 2020
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "font/ttf.h"

void TrueTypeFont::bind()
{
}

void TrueTypeFont::unbind()
{
}

void TrueTypeFont::setMaxAscent(int)
{
}
void TrueTypeFont::setMaxDescent(int)
{
}

int TrueTypeFont::getAdvance(char32_t ch) const
{
	return 0;
}

void TrueTypeFont::render(char32_t ch) const
{
}
void TrueTypeFont::render(const string &str) const
{
}

void TrueTypeFont::render(char32_t ch, float xoff, float yoff) const
{
}
void TrueTypeFont::render(const string &str, float xoff, float yoff) const
{
}


int TrueTypeFont::getWidth(const string &str) const
{
	return 0;
}

int TrueTypeFont::getMaxWidth() const
{
	return 0;
}

int TrueTypeFont::getHeight() const
{
	return 0;
}

int TrueTypeFont::getMaxAscent() const
{
	return 0;
}

int TrueTypeFont::getMaxDescent() const
{
	return 0;
}




