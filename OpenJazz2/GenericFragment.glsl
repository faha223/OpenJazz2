#version 110

uniform sampler2D Texture0;

varying vec4 color;

void main()
{
	vec4 base = texture2D(Texture0, gl_TexCoord[0].xy);
	
	gl_FragColor = base * color;
}