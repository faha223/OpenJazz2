#ifndef __SHADERS_H__
#define __SHADERS_H__

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

class Shaders
{
public:
	static GLuint CompileProgram(std::string vertexShaderSource, std::string fragmentShaderSource);
};

#endif