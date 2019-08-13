/*
 * render.cpp - OFS scene renderer package
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/render.h"

void Scene::init(int w, int h)
{
	int success;
	char infoLog[512];

	gl.init(w, h);


	const char *vsSource =
			"#version 330\n"
			"layout (location = 0) in vec3 aPos;\n"
			"\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\n";

	const char *fsSource =
			"#version 330\n"
			"out vec4 fragColor;\n"
			"\n"
			"void main()\n"
			"{\n"
			"   fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\n";

	uint32_t vsShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsShader, 1, &vsSource, nullptr);
	glCompileShader(vsShader);
	glGetShaderiv(vsShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vsShader, 512, nullptr, infoLog);
		cout << "Vertex Shader errors:\n" << infoLog << endl;
	}

	uint32_t fsShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsShader, 1, &fsSource, nullptr);
	glCompileShader(fsShader);
	glGetShaderiv(fsShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fsShader, 512, nullptr, infoLog);
		cout << "Fragment Shader errors:\n" << infoLog << endl;
	}

	shProgram = glCreateProgram();
	glAttachShader(shProgram, vsShader);
	glAttachShader(shProgram, fsShader);
	glLinkProgram(shProgram);

	glGetProgramiv(shProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shProgram, 512, nullptr, infoLog);
		cout << "Linking program errors:\n" << infoLog << endl;
	}

	glDeleteShader(vsShader);
	glDeleteShader(fsShader);


	// **********************************************************

	float vertices[] = {
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		 0.0f,  0.5f,  0.0f
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Scene::resize(int w, int h)
{
	gl.resize(w, h);
}

void Scene::render()
{
	gl.start();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shProgram);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	gl.finish();
}
