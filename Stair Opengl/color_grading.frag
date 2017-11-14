#version 440 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D lut;


void main()
{
	

	vec3 col = texture2D(screenTexture, TexCoords).rgb;

	vec2 colInLut;

	float rCeil;
	float rFloor;
	float rDiff;
	float rColorDiff;

	rCeil = (ceil(col.r * 16.0)) / 16.0;
	rFloor = (floor(col.r * 16.0)) / 16.0;
	rDiff = (col.r * 16.0) - floor(col.r * 16.0);

	float gCeil;
	float gFloor;
	float gDiff;
	float gColorDiff;

	gCeil = (ceil(col.g * 16.0)) / 16.0;
	gFloor = (floor(col.g * 16.0)) / 16.0;
	gDiff = (col.g * 16) - floor(col.g * 16.0);

	float bCeil;
	float bFloor;
	float bDiff;
	float bColorDiff;

	bCeil = (ceil(col.b * 16)) / 16.0;
	bFloor = (floor(col.b * 16)) / 16.0;
	bDiff = (col.b * 16) - floor(col.b * 16.0);


	colInLut.x = (rFloor + ceil(bFloor * 15.0)) / 16.0;
	colInLut.y = 1.0 - gFloor;
	vec3 color_floor = texture2D(lut, colInLut).rgb;

	colInLut.x = (rCeil + ceil(bCeil * 15.0)) / 16.0;
	colInLut.y = 1.0 - gCeil;
	vec3 color_ceil = texture2D(lut, colInLut).rgb;

	rColorDiff = (color_ceil.r - color_floor.r) * rDiff;
	gColorDiff = (color_ceil.g - color_floor.g) * gDiff;
	bColorDiff = (color_ceil.b - color_floor.b) * bDiff;
	
	color_floor.r += rColorDiff;
	color_ceil.g += gColorDiff;
	color_floor.b += bColorDiff;



	FragColor = vec4(color_floor.r, color_ceil.g, color_floor.b, 1.0);

}
