#ifndef __SHADERS_H__
#define __SHADERS_H__

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

class Shaders
{
public:
	static std::string CompileShader(GLenum type, const std::string &shaderSource, GLuint &out);
	static std::string LinkProgram(const GLuint &vertexShader, const GLuint &fragmentShader, GLuint &program);
};

#endif