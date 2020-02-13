/*
 * text.h - OpenGL text package (foreground overlay screen)
 *
 *  Created on: Jan 23, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "render/gl/font.h"

namespace ofs::gl {

};

class glText
{
public:
	glText() = default;
	~glText() = default;

	void begin();
	void end();

};
