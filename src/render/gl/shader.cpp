/*
 * shader.cpp - OFS shader compiler package
 *
 *  Created on: Aug 13, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/gl/shader.h"

void ShaderSource::compile(const string &name)
{
	auto vsName = fmt::sprintf("shaders/%s.vs", name);
	auto fsName = fmt::sprintf("shaders/%s.fs", name);

	ifstream vsFile, fsFile;
	stringstream vsSource, fsSource;
	string vsCode, fsCode;

	try {
		vsFile.open(vsName);
		fsFile.open(fsName);

		vsSource << vsFile.rdbuf();
		fsSource << fsFile.rdbuf();

		vsFile.close();
		fsFile.close();

		vsCode = vsSource.str();
		fsCode = fsSource.str();
	}
	catch( ifstream::failure e)
	{
		cout << "File not successful..." << endl;
	}

	int success;
	char infoLog[512];


	const char *vsrc = vsCode.c_str();
	uint32_t vsShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsShader, 1, &vsrc, nullptr);
	glCompileShader(vsShader);
	glGetShaderiv(vsShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vsShader, 512, nullptr, infoLog);
		cout << "Vertex Shader errors:\n" << infoLog << endl;
	}

	const char *fsrc = fsCode.c_str();
	uint32_t fsShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsShader, 1, &fsrc, nullptr);
	glCompileShader(fsShader);
	glGetShaderiv(fsShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fsShader, 512, nullptr, infoLog);
		cout << "Fragment Shader errors:\n" << infoLog << endl;
	}

	id = glCreateProgram();
	glAttachShader(id, vsShader);
	glAttachShader(id, fsShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
		cout << "Linking program errors:\n" << infoLog << endl;
	}

	glDeleteShader(vsShader);
	glDeleteShader(fsShader);
}

