#version 440 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D lut;

void main()
{
	vec3 col = texture2D(screenTexture, TexCoords).rgb;


	vec2 colInLut;

	col *= 255.0;

	colInLut.x = ((col.r * 15.0 / 256.0) + (col.b * 15.0 / 16.0)) / 256.0;
	colInLut.y = (255.0 - col.g )/ 256.0;

	vec3 color = texture2D(lut, colInLut.xy).rgb;

	FragColor = vec4(color.rgb, 1.0);
} 