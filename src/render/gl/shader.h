/*
 * shader.h - Shader compiler package
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#pragma once

struct LightState;

namespace ofs::gl {

};

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
	ShaderSource(ShaderType type);
	~ShaderSource();

	inline int getID() const { return id; }

	void dump(ostream &out, const std::string &source);
	const string getLogInfo();

	ShaderStatus compile(const vector<string> &source);

	static ShaderStatus create(ostream &out, ShaderType type,
		const vector<string>& source, ShaderSource **shader);

private:
	ShaderType type = ShaderType::shrUnknown;
	GLuint id = 0;
};

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	inline int getID() const { return id; }
	inline void use() const { glUseProgram(id); }
	inline void release() const { glUseProgram(0); }

	void attach(const ShaderSource &shader);

	const string getLogInfo();
	ShaderStatus link(ostream &out);

	void setLightParameters(LightState *ls, Color diffuse, Color specular, Color emissive);

private:
	GLuint id = 0;

	vec3f_t ambientColor;
	struct ShaderLight {
		vec3f_t direction;
		vec3f_t diffuse;
		vec3f_t specular;
	} lights[8];
};

class Context;
class ShaderManager
{
public:
	ShaderManager(const Context *gl);
	~ShaderManager() = default;

	ShaderStatus createProgram(ostream &out, const string &vsSource, const string &fsSource, ShaderProgram **pgm);
	ShaderProgram *buildPrograms(const string &vsSource, const string &fsSource);
	ShaderProgram *createShader(const string &name);

private:
	const Context *gl;
};
