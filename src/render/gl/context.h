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
	void init(int width, int height);

	inline int getWidth() const    { return width; }
	inline int getHeight() const   { return height; }
	inline float getAspect() const { return aspect; }

	void resize(int width, int height);

	// render routines
	void start();
	void finish();

protected:
    int   width  = 1;
	int   height = 1;
	float aspect = float(width)/float(height);

	unsigned int vao = 0;
	unsigned int vbo = 0;
	unsigned int ebo = 0;
};
