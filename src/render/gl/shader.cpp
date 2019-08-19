/*
 * shader.cpp - OFS shader compiler package
 *
 *  Created on: Aug 13, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/gl/shader.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

ShaderSource::ShaderSource(ShaderType type)
: type(type)
{
	GLenum idType;

	switch (type) {
	case shrVertexProcessor:
		idType = GL_VERTEX_SHADER;
		break;
	case shrFragmentProcessor:
		idType = GL_FRAGMENT_SHADER;
		break;
	}

	id = glCreateShader(idType);
}

ShaderSource::~ShaderSource()
{
	glDeleteShader(id);
}

void ShaderSource::dump(ostream &out, const string &source)
{
	bool newLine = true;
	int lineNumber = 0;

	for (int idx = 0; idx < source.length(); idx++) {
		if (newLine == true) {
			lineNumber++;
			out << setw(4) << lineNumber << ": ";
			newLine = false;
		}

		out << source[idx];
		if (source[idx] == '\n')
			newLine = true;
	}
	out << endl;

	out.flush();
}

const string ShaderSource::getLogInfo()
{
	GLint   lsize = 0;
	GLsizei size  = 0;

	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lsize);
	if (lsize <= 0)
		return string();

	char *clog = new char[lsize];
	if (clog == nullptr)
		return string();

	glGetShaderInfoLog(id, lsize, &size, clog);
	string slog(clog, size);
	delete [] clog;

	return slog;
}

ShaderStatus ShaderSource::compile(const vector<string> &source)
{
//	auto vsName = fmt::sprintf("shaders/%s.vs", name);
//	auto fsName = fmt::sprintf("shaders/%s.fs", name);
//
//	ifstream vsFile, fsFile;
//	stringstream vsSource, fsSource;
//	string vsCode, fsCode;
//
//	try {
//		vsFile.open(vsName);
//		fsFile.open(fsName);
//
//		vsSource << vsFile.rdbuf();
//		fsSource << fsFile.rdbuf();
//
//		vsFile.close();
//		fsFile.close();
//
//		vsCode = vsSource.str();
//		fsCode = fsSource.str();
//	}
//	catch( ifstream::failure e)
//	{
//		cout << "File not successful..." << endl;
//	}
//
//	int success;
//	char infoLog[512];
//

	GLint status;

	if (source.empty())
		return ShaderStatus::shrEmptyProgram;

	const char **src = new const char*[source.size()];
	for (unsigned int idx = 0; idx < source.size(); idx++)
		src[idx] = source[idx].c_str();

	glShaderSource(id, source.size(), src, nullptr);
	delete [] src;

	glCompileShader(id);

	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
		return ShaderStatus::shrCompileError;
	return ShaderStatus::shrSuccessful;
}

ShaderStatus ShaderSource::create(ostream &out, ShaderType type,
	const vector<string>& source, ShaderSource **shader)
{
	ShaderSource *newShader = new ShaderSource(type);
	ShaderStatus status;
	string log;

	if (newShader == nullptr)
		return ShaderStatus::shrOutOfMemory;

	for (unsigned int idx = 0; idx < source.size(); idx++)
		newShader->dump(out, source[idx]);

	status = newShader->compile(source);
	if (status != ShaderStatus::shrSuccessful) {
		log = newShader->getLogInfo();
		out << "\nCompiling shader source error:\n" << endl;
		out << log << endl;
		out.flush();

		delete newShader;
		return status;
	}

	*shader = newShader;
	return ShaderStatus::shrSuccessful;
}

// *************************************************************

ShaderProgram::ShaderProgram()
{
	id = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(id);
}

void ShaderProgram::attach(const ShaderSource &shader)
{
	glAttachShader(id, shader.getID());
}

const string ShaderProgram::getLogInfo()
{
	GLint   lsize = 0;
	GLsizei size  = 0;

	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &lsize);
	if (lsize <= 0)
		return string();

	char *clog = new char[lsize];
	if (clog == nullptr)
		return string();

	glGetProgramInfoLog(id, lsize, &size, clog);
	string slog(clog, size);
	delete [] clog;

	return slog;
}

ShaderStatus ShaderProgram::link(ostream &out)
{
	GLint  status;
	string log;

	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		log = getLogInfo();
		out << "\nLinking shader program error:\n" << endl;
		out << log << endl;

		return ShaderStatus::shrLinkError;
	}

	return ShaderStatus::shrSuccessful;
}

ShaderStatus ShaderManager::createProgram(ostream &out, const string &vsSource, const string &fsSource, ShaderProgram **pgm)
{
	vector<string> vsSourcev;
	vector<string> fsSourcev;
	ShaderSource *vsShader, *fsShader;
	ShaderStatus st;

	vsSourcev.push_back(vsSource);
	fsSourcev.push_back(fsSource);

	st = ShaderSource::create(out, shrVertexProcessor, vsSourcev, &vsShader);
	st = ShaderSource::create(out, shrFragmentProcessor, fsSourcev, &fsShader);

	ShaderProgram *npgm = new ShaderProgram();
	npgm->attach(*vsShader);
	npgm->attach(*fsShader);

	st = npgm->link(out);
	if (st == shrSuccessful)
		*pgm = npgm;

	return shrSuccessful;
}

ShaderProgram *ShaderManager::buildPrograms(const string &vsSource, const string &fsSource)
{
	ShaderProgram *pgm;
	ShaderStatus st;

	st = createProgram(cout, vsSource, fsSource, &pgm);
	if (st != ShaderStatus::shrSuccessful || pgm == nullptr)
		return nullptr;
	return pgm;
}

ShaderProgram *ShaderManager::createShader(const string &name)
{
	string vsSource, fsSource;

	auto vsName = fmt::sprintf("shaders/%s-vs.glsl", name);
	auto fsName = fmt::sprintf("shaders/%s-fs.glsl", name);

	struct stat st;

	if (!stat(vsName.c_str(), &st))
	{
		auto vsSize = st.st_size;
		ifstream vsFile(vsName);
		if (!vsFile.good()) {
			fmt::fprintf(cerr, "Failed to open '%s' file: %s\n",
				vsName, strerror(errno));
			return nullptr;
		}

		vsSource = string(vsSize, '\0');
		vsFile.read(&vsSource[0], vsSize);
		vsFile.close();
	}

	if (!stat(fsName.c_str(), &st))
	{
		auto fsSize = st.st_size;
		ifstream fsFile(fsName);
		if (!fsFile.good()) {
			fmt::fprintf(cerr, "Failed to open '%s' file: %s\n",
				fsName, strerror(errno));
			return nullptr;
		}

		fsSource = string(fsSize, '\0');
		fsFile.read(&fsSource[0], fsSize);
		fsFile.close();
	}

	return buildPrograms(vsSource, fsSource);
}
