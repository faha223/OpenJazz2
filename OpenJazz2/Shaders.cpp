#include "Shaders.h"
#include <algorithm>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

void LogError(const char *format, ...)
{
	if (format == nullptr)
		return;
	char buffer[1024];
	va_list ap;
	va_start(ap, format);
	vsprintf_s(buffer, 1023, format, ap);
	va_end(ap);

#if defined(_WIN32)|| defined(_WIN64)
	OutputDebugStringA(buffer);
#else
	fprintf(stderr, buffer);
#endif
}

using namespace std;

string CompileShader(GLenum type, const string &shaderSource, GLuint &out)
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

GLuint Shaders::CompileProgram(string vertexShaderSource, string fragmentShaderSource)
{
	GLuint vs = 0;
	GLuint fs = 0;
	string log = CompileShader(GL_VERTEX_SHADER, vertexShaderSource, vs);
	if (log.length() > 0)
	{
		LogError("Error compiling vertex shader:\n%s\n", log.c_str());
		return 0;
	}
	log = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource, fs);
	if (log.length() > 0)
	{
		LogError("Error compiling fragment shader:\n%s\n", log.c_str());
		return 0;
	}

	GLuint prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);

	glLinkProgram(prog);

	GLint blen = 0;
	glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &blen);
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
	if (log.length() > 0)
	{
		fprintf(stderr, "Error linking program: %s\n", log.c_str());
	}

	return prog;
}