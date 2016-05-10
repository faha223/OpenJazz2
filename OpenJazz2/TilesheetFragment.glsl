#version 330

varying vec2 v_texCoord;

uniform sampler2D Tilesheet;

void main()
{
	gl_FragColor = texture2D(Tilesheet, v_texCoord);
}