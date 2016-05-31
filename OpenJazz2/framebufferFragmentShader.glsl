#version 400

uniform sampler2D texture;

in vec4 color;
in vec2 texcoord;

out vec4 FragColor;

float roughStep(float x)
{
	float centeredAtZero = ((2.0 * clamp(x, 0.0, 0.99)) - 1.0);
	float curved = clamp(pow(centeredAtZero, 3) , -0.99, 0.99);
	return (curved/2.0) + 0.5;
}

#extension GL_EXT_gpu_shader4 : enable
vec2 adjustCoords(vec2 coord)
{
	vec2 tdim = textureSize2D(texture, 0);
	vec2 tc = vec2(tdim.x * coord.x, tdim.y * coord.y);
	float remainderx = tc.x - floor(tc.x);
	float remaindery = tc.y - floor(tc.y);
	tc.x = floor(tc.x) + roughStep(remainderx);
	tc.y = floor(tc.y) + roughStep(remaindery);
	return vec2(tc.x / tdim.x, tc.y / tdim.y);
}

void main()
{
	FragColor = texture2D(texture, adjustCoords(texcoord)) * color;
}