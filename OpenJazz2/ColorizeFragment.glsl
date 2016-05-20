#version 110

uniform sampler2D Texture0;

varying vec4 color;

vec3 rgb2hcv(vec3 rgb)
{
	float Epsilon = 1e-10;
	
    // Based on work by Sam Hocevar and Emil Persson
    vec4 P = (rgb.g < rgb.b) ? vec4(rgb.bg, -1.0, 2.0/3.0) : vec4(rgb.gb, 0.0, -1.0/3.0);
    vec4 Q = (rgb.r < P.x) ? vec4(P.xyw, rgb.r) : vec4(rgb.r, P.yzx);
    float C = Q.x - min(Q.w, Q.y);
    float H = abs((Q.w - Q.y) / (6.0 * C + Epsilon) + Q.z);
    return vec3(H, C, Q.x);
}

vec3 rgb2hsl(vec3 rgb)
{
	float Epsilon = 1e-10;
    vec3 HCV = rgb2hcv(rgb);
    float L = HCV.z - HCV.y * 0.5;
    float S = HCV.y / (1.0 - abs(L * 2.0 - 1.0) + Epsilon);
    return vec3(HCV.x, S, L);
}

vec3 hue2rgb(float H)
{
	float R = clamp(abs(H * 6.0 - 3.0) - 1.0, 0.0, 1.0);
    float G = clamp(2.0 - abs(H * 6.0 - 2.0), 0.0, 1.0);
    float B = clamp(2.0 - abs(H * 6.0 - 4.0), 0.0, 1.0);
    return vec3(R,G,B);
}

vec3 hsl2rgb(vec3 hsl) {
    vec3 rgb = hue2rgb(hsl.x);
	float C = (1.0 - abs(2.0 * hsl.z - 1.0)) * hsl.y;
	return (rgb - 0.5) * C + hsl.z;
}

void main()
{
	vec4 base = texture2D(Texture0, gl_TexCoord[0].xy);
	vec3 hsv = rgb2hsl(base.xyz);
	vec3 col = rgb2hsl(color.xyz);
	vec3 rgb = hsl2rgb(vec3(col.x, col.y, hsv.z * (255.0/195.0)));

	gl_FragColor = vec4(rgb, base.a * color.a);
}