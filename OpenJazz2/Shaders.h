#ifndef __SHADERS_H__
#define __SHADERS_H__

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include "System.h"

inline void LogAllGLErrors()
{
	GLuint error = glGetError();
	while (error != 0)
	{
		System::LogError("OpenGL Error: %d\n", error);
		error = glGetError();
	}
}

class Shaders
{
public:
	static std::string CompileShader(GLenum type, const std::string &shaderSource, GLuint &out);
	static std::string LinkProgram(const GLuint &vertexShader, const GLuint &fragmentShader, GLuint &program);
	static std::string GetShaderInfoLog(const GLuint &shader);
	static std::string GetProgramInfoLog(const GLuint &program);
};

#endif