/*
 * render.cpp - OFS scene renderer package
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/render.h"

#include <time.h>

void Scene::init(int w, int h)
{
	int success;
	char infoLog[512];

	gl.init(w, h);


	const char *vsSource =
			"#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec3 aColor;\n"
			"out vec3 vColor;\n"
			"\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos, 1.0);\n"
			"   vColor = aColor;\n"
			"}\n";

	const char *fsSource =
			"#version 330 core\n"
			"out vec4 fragColor;\n"
			"in vec3 vColor;\n"
			"\n"
			"void main()\n"
			"{\n"
			"   fragColor = vec4(vColor, 1.0);\n"
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

//	myColor = glGetUniformLocation(shProgram, "myColor");

	// **********************************************************
//	float vertices[] = {
//	    // positions
//	     0.5f, -0.5f, 0.0f,   // bottom right
//	    -0.5f, -0.5f, 0.0f,   // bottom left
//	     0.0f,  0.5f, 0.0f,   // top
//	};

	float vertices[] = {
	    // positions         // colors
	     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
	     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
	};

//	float vertices[] = {
//		// First triangle
//		 0.5f,  0.5f,  0.0f,
//		 0.5f, -0.5f,  0.0f,
//		-0.5f, -0.5f,  0.0f,
//		-0.5f,  0.5f,  0.0f
//	};
//	uint32_t indices[] = {
//		0, 1, 3,
//		1, 2, 3
//	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
//	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

	time_t now = time(nullptr);
	float green = (sin(now) / 2.0f) + 0.5f;

	glUseProgram(shProgram);
//	glUniform4f(myColor, 0.0f, green, 0.0f, 1.0f);

	glBindVertexArray(vao);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	gl.finish();
}
