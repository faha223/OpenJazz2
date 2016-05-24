#version 400

uniform sampler2D texture;

in vec4 color;
in vec2 texcoord;

out vec4 FragColor;

void main()
{
	FragColor = texture2D(texture, texcoord) * color;
}