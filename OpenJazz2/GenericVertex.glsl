#version 400

uniform mat4 modelview;
uniform mat4 projection;
uniform vec4 VertexColor;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexcoord;

out vec4 color;
out vec2 texcoord;

void main()
{
	gl_Position = projection * modelview * vec4(VertexPosition, 1.0);
	color = VertexColor;
	texcoord = VertexTexcoord;
}