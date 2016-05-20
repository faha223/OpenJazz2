#include "Shaders.h"

using namespace std;

string Shaders::CompileShader(GLenum type, const string &shaderSource, GLuint &out)
{
	out = glCreateShader(type);
	string log;

	int ivl = shaderSource.length();
	char *vtemp = new char[ivl];
	memcpy(vtemp, shaderSource.c_str(), ivl);
	glShaderSource(out, 1, &vtemp, &ivl);
	glCompileShader(out);

	GLint blen = 0;
	glGetShaderiv(out, GL_INFO_LOG_LENGTH, &blen);
	if (blen > 1)
	{
		GLsizei slen = 0;
		GLchar *compiler_log = (GLchar*)malloc(blen);
		glGetInfoLogARB(out, blen, &slen, compiler_log);
		log = string(compiler_log);
		free(compiler_log);
		glDeleteShader(out);
		out = 0;
	}

	return log;
}

string Shaders::LinkProgram(const GLuint &vertexShader, const GLuint &fragmentShader, GLuint &prog)
{
	prog = glCreateProgram();
	glAttachShader(prog, vertexShader);
	glAttachShader(prog, fragmentShader);
	glLinkProgram(prog);

	GLint blen = 0;
	glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &blen);
	string log;
	if (blen > 1)
	{
		GLsizei slen = 0;
		GLchar *compiler_log = (GLchar*)malloc(blen);
		glGetInfoLogARB(prog, blen, &slen, compiler_log);
		log = string(compiler_log);
		free(compiler_log);
		glDeleteProgram(prog);
		prog = 0;
	}

	return log;
}