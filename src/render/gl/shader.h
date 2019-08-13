/*
 * shader.h - Shader compiler package
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/gl/context.h"

enum ShaderType {
	shrUnknown = 0,
	shrVertexProcessor,
	shrFragmentProcessor
};

enum ShaderStatus {
	shrSuccessful,
	shrCompileError,
	shrLinkError,
	shrOutOfMemory,
	shrEmptyProgram
};

class ShaderSource
{
public:
	ShaderSource() = default;
	~ShaderSource() = default;

	void compile(const string &name);

private:
	ShaderType type = ShaderType::shrUnknown;
	GLuint id = -1;
};

class ShaderProgram
{
public:
	ShaderProgram() = default;
	~ShaderProgram() = default;

	inline int getID() const { return id; }

private:
	int id = -1;
};

class ShaderPackage
{
public:
	ShaderPackage() = default;
	~ShaderPackage() = default;

};

class ShaderManager
{
public:
	ShaderManager() = default;
	~ShaderManager() = default;

};
