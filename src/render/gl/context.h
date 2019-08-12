/*
 * context.h
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#pragma once

#include <GL/glew.h>

class Context
{
public:
	enum vaType {
		glVAO,
		glVBO, // vertex array buffer data
		glEBO  // element array buffer data
	};

	Context() = default;
	~Context() = default;

	// initialization routines
	void init();

	// render routines
	void start();
	void finish();

private:
	unsigned int vao = 0;
	unsigned int vbo = 0;
	unsigned int ebo = 0;
};
