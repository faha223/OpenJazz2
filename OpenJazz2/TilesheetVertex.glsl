#version 330

uniform mat4 modelview;
uniform mat4 projection;

attrib vec3 a_vertex;
attrib vec2 a_texCoord;

varying vec2 v_texCoord;

void main()
{
	gl_Position = projection * modelview * vec4(a_vertex, 1.0);
	v_texCoord = texCoord;
}