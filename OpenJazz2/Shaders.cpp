#include "Shaders.h"

using namespace std;

string Shaders::CompileShader(GLenum type, const string &shaderSource, GLuint &out)
{
	switch (type)
	{
	case GL_VERTEX_SHADER:
	case GL_FRAGMENT_SHADER:
	case GL_GEOMETRY_SHADER:
	case GL_TESS_CONTROL_SHADER:
	case GL_TESS_EVALUATION_SHADER:
		break;
	default:
		return "Invalid Shader Type";
	}

	if (shaderSource.length() == 0)
		return "You must provide source code to a glsl shader";

	out = glCreateShader(type);
	if (out == 0)
		return "Unable to create a shader";

	int ivl = shaderSource.length();
	char *vtemp = new char[ivl];
	memcpy(vtemp, shaderSource.c_str(), ivl);
	glShaderSource(out, 1, &vtemp, &ivl);
	glCompileShader(out);

	string log = GetShaderInfoLog(out);
	if (log.length() > 0)
	{
		glDeleteShader(out);
		out = 0;
	}

	return log;
}

string Shaders::GetShaderInfoLog(const GLuint &shader)
{
	string log;
	GLint blen = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &blen);
	if (blen > 1)
	{
		GLsizei slen = 0;
		GLchar *compiler_log = (GLchar*)malloc(blen);
		glGetInfoLogARB(shader, blen, &slen, compiler_log);
		log = string(compiler_log);
		free(compiler_log);
	}
	return log;
}

string Shaders::LinkProgram(const GLuint &vertexShader, const GLuint &fragmentShader, GLuint &prog)
{
	prog = 0;

	if (!glIsShader(vertexShader))
		return "Vertex Shader must be a shader";
	if (!glIsShader(fragmentShader))
		return "Fragment Shader must be a shader";

	prog = glCreateProgram();

	if (prog == 0)
		return "Unable to create program";

	glAttachShader(prog, vertexShader);
	glAttachShader(prog, fragmentShader);
	glLinkProgram(prog);

	string log = GetProgramInfoLog(prog);
	if (log.length() > 0)
	{
		glDeleteProgram(prog);
		prog = 0;
	}

	return log;
}

string Shaders::GetProgramInfoLog(const GLuint &program)
{
	string log;
	GLint blen = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &blen);

	if (blen > 1)
	{
		GLsizei slen = 0;
		GLchar *compiler_log = (GLchar*)malloc(blen);
		glGetProgramInfoLog(program, blen, &slen, compiler_log);
		log = string(compiler_log);
		free(compiler_log);
	}
	return log;
}