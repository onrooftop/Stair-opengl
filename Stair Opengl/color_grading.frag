#version 440 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D lut;
const float offset = 1.0;  

void main()
{
	 vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
       vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
	);
	vec3 color = vec3(0);
	for(int i = 0; i < 9; i++)
	{
		vec3 col = texture2D(screenTexture, TexCoords + offsets[i]).rgb;
		//vec3 col = texture2D(screenTexture, TexCoords).rgb;

		vec2 colInLut;

		col *= 255.0;

		colInLut.x = ((col.r * 15.0 / 255.0) + (col.b * 15.0 / 16.0)) / 255.0;
		colInLut.y = (255.0 - col.g )/ 255.0;

		color += texture2D(lut, colInLut.xy).rgb;

	}
	color /= 9.0;

	FragColor = vec4(color.rgb, 1.0);
} 