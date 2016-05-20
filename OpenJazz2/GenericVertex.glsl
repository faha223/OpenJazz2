#version 110

varying vec4 color;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	color = gl_Color;
}